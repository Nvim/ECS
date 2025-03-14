{
  description = "C++ ECS library";

  inputs = { };

  outputs =
    {
      nixpkgs,
      ...
    }:
    let
      system = "x86_64-linux";
      pkgs = import nixpkgs {
        inherit system;
        config.allowUnfree = true;
      };
      lib = pkgs.lib;
    in
    {
      devShells.${system}.default = pkgs.mkShell.override { stdenv = pkgs.clang18Stdenv; } {
        packages = with pkgs; [
          # IDE/Dev Tools:
          cmake
          pkg-config
          clang-tools_18 # LSP & Formatter
          lldb_18 # Debugger
          gdbgui
        ];
      };
    };
}
