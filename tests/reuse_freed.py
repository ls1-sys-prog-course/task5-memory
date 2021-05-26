#!/usr/bin/env python3

import os
import tempfile

from testsupport import run, run_project_executable, subtest, test_root, warn, ensure_library

def main() -> None:
    # Get test abspath
    testname = ["alloc_free_medium", "calloc_free_medium", "alloc_realloc_free_medium"]
    lib = ensure_library("libmymalloc.so")
    with tempfile.TemporaryDirectory() as tmpdir:

        for test in testname:
            test_file = test_root().joinpath(test)
            if not test_file.exists():
                run(["make", "-C", str(test_root()), str(test_root())+"/"+str(test)])
        
        # Check that libmymalloc.so provides malloc,free,calloc,realloc symbols
        with subtest("Check that malloc,realloc,free,calloc functions are available in libmymalloc.so"):
            with open(f"{tmpdir}/stdout", "w+") as stdout:
                run(["nm", "-D", "--defined-only", str(lib)], stdout=stdout)
            ok = 0
            with open(f"{tmpdir}/stdout", "r") as stdout:
                for l in stdout.readlines():
                    if l.endswith(" T malloc\n"):
                        ok += 1
                    elif l.endswith(" T realloc\n"):
                        ok += 1
                    elif l.endswith(" T free\n"):
                        ok += 1
                    elif l.endswith(" T calloc\n"):
                        ok += 1
                if ok != 4:
                    warn(f"{str(lib)} is not providing malloc,realloc,free,calloc!")
                    exit(1)
        
        #run malloc/calloc/free test with preloaded custom lib
        for test in testname:
            test_file = test_root().joinpath(test)
            with subtest(f"Run {test} with {lib} preloaded"):
                run([str(test_file)], extra_env={"LD_PRELOAD": str(lib)})
            
if __name__ == "__main__":
    main()
