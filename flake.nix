{
  description = "LLVM Build Flake";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-25.11";
  };

  outputs =
    { self, nixpkgs, ... }:
    let
      system = "x86_64-linux";
      pkgs = nixpkgs.legacyPackages.${system};

      conanOverride = pkgs.conan.overridePythonAttrs (oldAttrs: {
        doCheck = false;
      });

      devPackages = with pkgs; [
        automake
        autoconf
        ccache
        cmake
        conanOverride
        gperf
        gnumake
        libtool
        nasm
        m4
        perl
        python3
        ruby_3_4
        rake
        stdenv
      ];

      buildStdenv = with pkgs; overrideCC stdenv pkgs.llvmPackages.clangUseLLVM;

    in
    {
      devShells.${system}.default =
        pkgs.mkShell.override
          {
            stdenv = buildStdenv;
          }
          {
            packages =
              with pkgs.llvmPackages;
              [
                bintools
                clang-tools
                lldb
                lld
                llvm
              ]
              ++ devPackages;

            hardeningDisable = [ "all" ];
            impureUseNativeOptimizations = true;

            NIX_ENFORCE_NO_NATIVE = false;
            NIX_HARDENING_DISABLE = true;
            LLVM_ROOT = "${buildStdenv.cc}";
            LLVM_LIB = "${buildStdenv.cc.libcxx}/lib";
          };
    };
}
