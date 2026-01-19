
{
  description = "ARM cross-compilation and embedded development environment";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
  };

  outputs = { self, nixpkgs }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs { inherit system; };
    in {
      devShells.${system}.default = pkgs.mkShell {
        packages = with pkgs; [
          # ARM Linux (armhf) cross toolchain
          pkgsCross.armv7l-hf-multiplatform.buildPackages.gcc
          pkgsCross.armv7l-hf-multiplatform.glibc
          pkgsCross.armv7l-hf-multiplatform.libgcc

          # Bare-metal ARM toolchain (includes binutils)
          gcc-arm-embedded

          # Debugging / flashing
          openocd
        ];
      };
    };
}

