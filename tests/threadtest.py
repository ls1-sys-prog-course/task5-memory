#!/usr/bin/env python3

import os
import tempfile

from testsupport import run, run_project_executable, subtest, test_root, warn, ensure_library


def main() -> None:
    # Get test abspath
    min_scaling_factor = 1.2
    testname = "threadtest"
    lib = ensure_library("libmymalloc.so")
    with tempfile.TemporaryDirectory() as tmpdir:
        test = test_root().joinpath(testname)
        if not test.exists():
            run(["make", "-C", str(test_root()), str(test)])
        
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
        exec_time = []
        for threads in num_of_threads:
            with subtest(f"Run {testname} with {lib} preloaded with {threads} threads"):
                with open(f"{tmpdir}/stdout", "w+"
                    ) as stdout:
                    run_project_executable(
                            str(test),
                            args=[str(threads)],
                            stdout=stdout,
                            extra_env={"LD_PRELOAD": str(lib)}
                            )
                    if os.stat(f"{tmpdir}/stdout").st_size > 0:
                        exec_time.append(float(open(f"{tmpdir}/stdout").readlines()[0].strip()))
                    if os.stat(f"{tmpdir}/stdout").st_size == 0:
                        warn("Test failed with error")
                        exit(1)
        
        s1_2 = exec_time[0] / exec_time[1]
        s2_4 = exec_time[1] / exec_time[2]

        if (s1_2 < min_scaling_factor or s1_2 > 2 or s2_4 < min_scaling_factor or s2_4 > 2):
            warn("Allocator does not scale properly. Execution times : " + str(exec_time))
            exit(1)
        
if __name__ == "__main__":
    main()
