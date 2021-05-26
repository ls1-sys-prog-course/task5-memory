#!/usr/bin/env python3

import os
import tempfile

from testsupport import run, run_project_executable, subtest, test_root, warn, ensure_library


def main() -> None:
    # Get test abspath
    min_scaling_factor = 1.2
    testname = ["threadtest", "larson"]
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
        
        num_of_threads = [1, 2, 4]
        res = []

        for test in testname:
            test_file = test_root().joinpath(test)
            for threads in num_of_threads:
                with subtest(f"Run {test} with {lib} preloaded with {threads} thread(s)"):
                    with open(f"{tmpdir}/stdout", "w+"
                        ) as stdout:
                        run_project_executable(
                                str(test_file),
                                args=[str(threads)],
                                stdout=stdout,
                                extra_env={"LD_PRELOAD": str(lib)}
                                )
                        if os.stat(f"{tmpdir}/stdout").st_size > 0:
                            res.append(float(open(f"{tmpdir}/stdout").readlines()[0].strip()))
                        if os.stat(f"{tmpdir}/stdout").st_size == 0:
                            warn("Test failed with error")
                            exit(1)
            
            if (test=="threadtest"):
                s1_2 = res[0] / res[1]
                s2_4 = res[1] / res[2]
                if (s1_2 < min_scaling_factor or s2_4 < min_scaling_factor):
                    warn("Allocator does not scale properly. Execution times : " + str(res))
                    exit(1)
                else:
                    res = []
            elif (test=="larson"):
                s1_2 = res[1] / res[0]
                s2_4 = res[2] / res[1]
                if (s1_2 < min_scaling_factor or s2_4 < min_scaling_factor):
                    warn("Allocator does not scale properly. Throughput : " + str(res))
                    exit(1)
                else:
                    res = []
        
if __name__ == "__main__":
    main()
