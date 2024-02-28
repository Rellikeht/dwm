{
  description = "Rellikeht's build of suckless dwm";

  inputs = {
    nixpkgs.url = github:NixOS/nixpkgs;
    flake-utils.url = github:numtide/flake-utils;
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachSystem [
      "x86_64-linux"
      "i686-linux"
      "aarch64-linux"
      "armv7l-linux"
    ] (system: let
      pkgs = nixpkgs.legacyPackages.${system};
      name = "dwm";
      src = self;
    in {
      packages.default = pkgs.stdenv.mkDerivation rec {
        inherit name system src;

        PREFIX = "$(out)";
        CC = pkgs.gcc;
        PKG_CONFIG = pkgs.pkg-config;

        nativeBuildInputs = with pkgs; [
          pkg-config
        ];

        buildInputs = with pkgs; [
          xorg.libX11
          xorg.libXft
          xorg.libXinerama
          #          freetype
        ];

        buildPhase = "
            make clean
            make
            ";

        installPhase = "
          mkdir -p $out/bin
          make install
          ";
      };
    });
}
