"""Verificacoes pequenas, executaveis localmente e no GitHub Actions."""
import pathlib
import subprocess
import tempfile


def run(*args, **kwargs):
    return subprocess.run(args, check=True, **kwargs)


def main():
    # Compara todos os arquivos rastreados com a arvore vazia, inclusive no bootstrap.
    empty_tree = run(
        "git", "hash-object", "-t", "tree", "--stdin",
        input="", text=True, capture_output=True,
    ).stdout.strip()
    run("git", "diff", "--check", empty_tree)

    paths = run("git", "ls-files", "-z", capture_output=True).stdout
    sources = [
        pathlib.Path(p.decode("utf-8"))
        for p in paths.split(b"\0") if p and p.endswith(b".c")
    ]
    if not sources:
        print("Sem arquivos C: verificacao de compilacao ainda nao aplicavel.", flush=True)
        return

    with tempfile.TemporaryDirectory(prefix="pe-check-") as output:
        for index, source in enumerate(sources):
            print(f"Compilando {source}", flush=True)
            run(
                "gcc", "-std=c90", "-pedantic-errors", "-Wall", "-Wextra",
                "-I.", "-Iinclude", "-Isrc", "-c", str(source),
                "-o", str(pathlib.Path(output) / f"{index}.o"),
            )
    print(f"{len(sources)} arquivo(s) C compilado(s).", flush=True)


if __name__ == "__main__":
    main()