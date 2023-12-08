{
  description = "Rellikeht's build of suckless dwm";

  inputs.nixpkgs.url = github:NixOS/nixpkgs;

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
  in {
    packages.x86_64-linux.default = with import nixpkgs {system = system;};
      stdenv.mkDerivation rec {
        name = "dwm";
        src = self;

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
  };
}
