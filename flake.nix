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
        impureUseNativeOptimizations = true;
        # dontStrip = true;

        nativeBuildInputs = with pkgs; [
          pkg-config
        ];

        buildInputs = with pkgs; [
          libX11
          libXft
          libXinerama
          freetype
        ];

        buildPhase = ''
          sh patch.sh
          cd patched
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
