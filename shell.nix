{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    buildInputs = with pkgs;
     [
      clang-tools
      cmake
      codespell
      cppcheck
      gtest
      flex
      bison
      graphviz
    ];
}