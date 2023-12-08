{
  description = "Rellikeht's build of suckless st";

  inputs.nixpkgs.url = github:NixOS/nixpkgs;

  outputs = {
    self,
    nixpkgs,
  }: let
    system = "x86_64-linux";
  in {
    packages.x86_64-linux.default = with import nixpkgs {system = system;};
      stdenv.mkDerivation rec {
        name = "st";
        src = self;

        PREFIX = "$(out)";
        CC = pkgs.gcc;
        PKG_CONFIG = pkgs.pkg-config;
        # TERMINFO = "$out/share/terminfo";

        nativeBuildInputs = with pkgs; [
          pkg-config
        ];

        buildInputs = with pkgs; [
          xorg.libX11
          xorg.libXft
          xorg.libXinerama
          freetype
          ncurses
        ];

        buildPhase = "
            make clean
            make
            ";

        installPhase = "
          export TERMINFO=\"$out/share/terminfo\"
          mkdir -p $out/bin
          mkdir -p $TERMINFO
          make install
          ";
      };
  };
}
