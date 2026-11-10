#!/usr/bin/env python3

import argparse
import json
import math
import re
import subprocess
from pathlib import Path


def parse_expected(output):
    values = {}
    for quantile, value in re.findall(r"Expected\s+(2\.5|16\.0|50\.0|84\.0|97\.5)%:\s+r <\s+([-+\d.eE]+)", output):
        if quantile in values:
            raise ValueError("Duplicate expected quantile " + quantile)
        values[quantile] = float(value)
    ordered = [values[q] for q in ("2.5", "16.0", "50.0", "84.0", "97.5")]
    if not all(math.isfinite(v) and v > 0 for v in ordered) or ordered != sorted(ordered):
        raise ValueError("Invalid expected limits")
    return ordered


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--datacard", default="datacard.root")
    parser.add_argument("--masses", nargs="+", type=int, default=list(range(500, 1501, 100)))
    parser.add_argument("--parametric", action="store_true")
    args = parser.parse_args()
    if len(args.masses) > 1 and "{mass}" not in args.datacard and not args.parametric:
        parser.error("Template scans require a mass-specific --datacard path containing {mass}")
    results = []
    failed = False
    with open("result.txt", "w") as diagnostics, open("higgsLimits.txt", "w") as limits:
        limits.write("mass    median Limit    1_Sigma Lower   1_Sigma Upper   2_Sigma Lower   2_Sigma Upper\n")
        for mass in args.masses:
            command = ["combine", args.datacard.format(mass=mass), "-M", "AsymptoticLimits",
                       "--run", "blind", "-m", str(mass), "-n", f"HiggsCombine{mass}"]
            frozen = []
            if args.parametric:
                frozen.extend(["realHiggsMass", "b_ee", "b_eu"])
                command.extend(["--setParameters", f"realHiggsMass={mass},b_ee=1,b_eu=1"])
            if frozen:
                command.extend(["--freezeParameters", ",".join(frozen)])
            record = {"mass": mass, "command": command}
            try:
                result = subprocess.run(command, capture_output=True, text=True)
                record["exit_code"] = result.returncode
                diagnostics.write(f"mass={mass} exit_code={result.returncode}\n{result.stdout}\n{result.stderr}\n")
                Path(f"combine_{mass}.log").write_text(result.stdout + "\n" + result.stderr)
                if result.returncode:
                    raise RuntimeError(f"Combine exited with {result.returncode}")
                q = parse_expected(result.stdout)
                record["expected"] = dict(zip(("0.025", "0.16", "0.5", "0.84", "0.975"), q))
                limits.write(f"{mass:<7} {q[2]:<15g} {q[1]:<15g} {q[3]:<15g} {q[0]:<15g} {q[4]:<15g}\n")
                print(mass, record["expected"], flush=True)
            except (OSError, RuntimeError, ValueError, KeyError) as error:
                failed = True
                record["error"] = str(error)
                diagnostics.write(f"mass={mass} error={error}\n")
                print(f"FAILED mass={mass}: {error}", flush=True)
            results.append(record)
            Path("combine_results.json").write_text(json.dumps(results, indent=2) + "\n")
    return int(failed)


if __name__ == "__main__":
    raise SystemExit(main())
