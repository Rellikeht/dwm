{
  description = "Rellikeht's build of suckless dwm";

  inputs = {
    nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";
    flake-utils.url = "github:numtide/flake-utils";
  };

  outputs = {
    self,
    nixpkgs,
    flake-utils,
  }:
    flake-utils.lib.eachSystem
    flake-utils.lib.allSystems
    # [
    #   "x86_64-linux"
    #   "i686-linux"
    #   "aarch64-linux"
    #   "armv7l-linux"
    # ]
    (system: let
      pkgs = nixpkgs.legacyPackages.${system};
      lib = pkgs.lib;
      name = "dwm";
      src = self;
    in {
      packages.default = pkgs.stdenv.mkDerivation {
        inherit name system src;

        PREFIX = "$(out)";
        CC = pkgs.stdenv.cc;
        PKG_CONFIG = pkgs.pkg-config;

        nativeBuildInputs = with pkgs; [
          pkg-config
        ];

        buildInputs = with pkgs; [
          xorg.libX11
          xorg.libXft
          xorg.libXinerama
          # freetype
        ];

        buildPhase = ''
          make clean
          make
        '';

        installPhase = let
          # Hopefully this shit will work
          sesDir = "$out/share/xsessions";
        in ''
          mkdir -p $out/bin ${sesDir}
          make install
          cp dwm.desktop ${sesDir}
        '';

        meta = with lib; {
          homepage = "https://dwm.suckless.org/";
          description = "Dynamic window manager, Rellikeht's build";
          license = licenses.mit;
          mainProgram = "dwm";
          maintainers = ["Rellikeht"];
          platforms = platforms.linux;
        };
      };
    });
}
