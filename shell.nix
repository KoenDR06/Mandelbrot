{ pkgs ? import <nixpkgs> {} }:

pkgs.mkShell {
  nativeBuildInputs = with pkgs; [
    gcc
  ];
  buildInputs = with pkgs; [
    cimg
    xorg.libX11
    xorg.libXrandr
    xorg.libXrender
    xorg.libXext
    jetbrains.clion
    ];

  shellHook = ''
    alias compile="time g++ src/main.cpp -o bin/dev -lX11 -lXrandr -lXrender -lXext && echo 'Compiling finished.' && ./bin/dev"
  '';
}

