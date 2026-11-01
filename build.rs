use std::env;
use std::path::PathBuf;

fn main() {
    // Retrieve the absolute path of the package manifest directory.
    let manifest_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let manifest_path = PathBuf::from(&manifest_dir);

    // Locate C source and include directories relative to the crate root.
    let include_dir = manifest_path.join("include");
    let src_uart = manifest_path.join("src").join("uart.c");

    // Tell Cargo to re-run this build script if any C header or source file changes.
    println!("cargo:rerun-if-changed=include/rp2350_hal.h");
    println!("cargo:rerun-if-changed=include/uart.h");
    println!("cargo:rerun-if-changed=include/serial.h");
    println!("cargo:rerun-if-changed=include/helpers.h");
    println!("cargo:rerun-if-changed=src/uart.c");

    // Export the include directory path to dependent crates via Cargo metadata.
    println!("cargo:include={}", include_dir.display());

    // Initialize the C compiler builder.
    let mut build = cc::Build::new();
    build
        .file(&src_uart)
        .include(&include_dir)
        // Bare-metal flag: assume no standard C library or operating system runtime.
        .flag("-ffreestanding")
        // Enforce strict compiler warnings and treat all warnings as build errors.
        .flag("-Wall")
        .flag("-Wextra")
        .flag("-Werror");

    // Check if we are targetting a 32-bit RISC-V architecture.
    let target = env::var("TARGET").unwrap_or_default();
    if target.contains("riscv32") {
        // Auto-detect a suitable RISC-V GCC cross-compiler if CC is not explicitly set.
        if env::var("CC").is_err() && env::var("CC_riscv32imac_unknown_none-elf").is_err() {
            let candidates = [
                "riscv-none-elf-gcc",
                "riscv64-unknown-elf-gcc",
                "riscv32-unknown-elf-gcc",
                "riscv64-linux-gnu-gcc",
            ];

            for compiler in candidates {
                if std::process::Command::new(compiler)
                    .arg("--version")
                    .output()
                    .is_ok()
                {
                    build.compiler(compiler);
                    break;
                }
            }
        }

        // Apply RP2350 Hazard3 RISC-V architecture-specific compiler flags:
        // - march: RV32IMAC base + Zicsr/Zifencei control extensions + Zba/Zbb/Zbkb/Zbs Bit Manipulation extensions.
        // - mabi: ILP32 standard 32-bit integer ABI.
        // - mcmodel: medlow memory model (code and data residing in the lower 2GB address space).
        build
            .flag("-march=rv32imac_zicsr_zifencei_zba_zbb_zbkb_zbs")
            .flag("-mabi=ilp32")
            .flag("-mcmodel=medlow");
    }

    // Compile the C source files into a static library named `librp2350-hazard3-hal.a`.
    build.compile("rp2350-hazard3-hal");

    // Generate dynamic Rust FFI bindings from C header files using bindgen.
    let bindings = bindgen::Builder::default()
        .header(include_dir.join("rp2350_hal.h").to_str().unwrap())
        .clang_arg(format!("-I{}", include_dir.display()))
        // Ensure no_std compatibility by resolving C types from core instead of std.
        .use_core()
        // Wrap raw unsafe operations within unsafe blocks for modern Rust edition compliance.
        .wrap_unsafe_ops(true)
        // Map C primitive types to core::ffi primitives.
        .ctypes_prefix("core::ffi")
        // Suppress lints on generated FFI code.
        .raw_line("#[allow(warnings)]")
        .raw_line("#[allow(unsafe_op_in_unsafe_fn)]")
        .generate()
        .expect("Unable to generate C bindings");

    // Write the generated bindings to `bindings.rs` inside Cargo's output directory.
    let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
    bindings
        .write_to_file(out_path.join("bindings.rs"))
        .expect("Unable to write bindings.rs");
}