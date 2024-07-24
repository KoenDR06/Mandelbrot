{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = [
    pkgs.gcc
  ];
  buildInputs = [
    pkgs.cimg
    pkgs.xorg.libX11
    pkgs.xorg.libXrandr
    pkgs.xorg.libXrender
    pkgs.xorg.libXext
  ];

  shellHook = ''
    alias compile="time g++ src/main.cpp -o bin/dev -lX11 -lXrandr -lXrender -lXext && echo 'Compiling finished.' && ./bin/dev"
  '';
}

