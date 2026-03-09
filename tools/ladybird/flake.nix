{
  description = "LibWeb-to-YDraw - Ladybird LibWeb integration for Yetty";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixos-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = { self, nixpkgs, flake-utils }:
    flake-utils.lib.eachDefaultSystem (system:
      let
        pkgs = import nixpkgs { inherit system; };

        # Ladybird source - fetched from GitHub
        ladybirdSrc = pkgs.fetchFromGitHub {
          owner = "LadybirdBrowser";
          repo = "ladybird";
          rev = "master";
          sha256 = "sha256-quSuJ4shaYP9yVzyijtmRx5gxCG8rMWpAWVh7t6GzwM=";
        };

        # Public suffix list - pre-fetched for sandboxed builds
        publicSuffixList = pkgs.fetchurl {
          url = "https://publicsuffix.org/list/public_suffix_list.dat";
          sha256 = "sha256-3U4yZdoVr2nr6WOj2tbNBUEW3l9oeAPa8ZluaXhqqbE=";
        };

        libweb-to-ydraw = pkgs.stdenv.mkDerivation {
          pname = "libweb-to-ydraw";
          version = "0.1.0";

          src = pkgs.lib.cleanSource ./.;

          # Don't unpack ladybird into the build dir
          dontUnpack = false;

          nativeBuildInputs = with pkgs; [
            cmake
            ninja
            pkg-config
            python3
          ];

          buildInputs = with pkgs; [
            # Core dependencies
            fmt
            simdutf
            curl
            icu
            fast-float
            harfbuzz
            fontconfig
            freetype

            # Image libraries
            libavif
            simdjson
            libjpeg
            libpng
            libwebp
            libjxl
            libtiff
            giflib

            # XML
            libxml2
          ];

          # Pre-copy required files before configure (nix sandbox blocks network)
          preConfigure = ''
            cp ${publicSuffixList} public_suffix_list.dat
          '';

          cmakeFlags = [
            "-DBUILD_LIBWEB=ON"
            "-DLADYBIRD_SOURCE_DIR=${ladybirdSrc}"
          ];

          # Install
          installPhase = ''
            mkdir -p $out/bin $out/lib
            find . -maxdepth 1 -type f -executable -exec cp {} $out/bin/ \;
            find . -name "*.a" -exec cp {} $out/lib/ \;
          '';

          # Use all cores
          enableParallelBuilding = true;

          meta = with pkgs.lib; {
            description = "LibWeb to YDraw converter for Yetty terminal";
            license = licenses.bsd2;
            platforms = platforms.linux;
          };
        };

      in {
        packages = {
          default = libweb-to-ydraw;
          libweb-to-ydraw = libweb-to-ydraw;
        };

        devShells.default = pkgs.mkShell {
          inputsFrom = [ libweb-to-ydraw ];

          packages = with pkgs; [
            ccache
          ];
        };
      }
    );
}
