import importlib.util
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

SCRIPT = Path(__file__).resolve().parents[1] / "runCombineCommands.py"
spec = importlib.util.spec_from_file_location("runner", SCRIPT)
runner = importlib.util.module_from_spec(spec)
spec.loader.exec_module(runner)
OUTPUT = "\n".join(f"Expected {q}%: r < {v}" for q, v in
                   [("2.5", "1e-3"), ("16.0", "2e-3"), ("50.0", "3e-3"), ("84.0", "4e-3"), ("97.5", "5e-3")])


class RunnerTests(unittest.TestCase):
    def test_quantiles(self):
        self.assertEqual(runner.parse_expected(OUTPUT), [.001, .002, .003, .004, .005])
        for bad in ["", OUTPUT+OUTPUT, OUTPUT.replace("50.0", "51.0"), OUTPUT.replace("3e-3", "nan"), OUTPUT.replace("3e-3", "8e-3")]:
            with self.assertRaises((ValueError, KeyError)):
                runner.parse_expected(bad)

    def test_scan_rejects_reused_template(self):
        result = subprocess.run([sys.executable, str(SCRIPT), "--masses", "500", "600"], capture_output=True, text=True)
        self.assertEqual(result.returncode, 2)
        self.assertIn("mass-specific", result.stderr)

    def test_execution(self):
        with tempfile.TemporaryDirectory() as tmp:
            command = Path(tmp) / "combine"
            command.write_text("#!/usr/bin/env python3\nimport sys\nm = sys.argv[sys.argv.index('-m')+1]\nprint(" + repr(OUTPUT) + ")\nif m == '600':\n    print('fixture failure', file=sys.stderr)\n    sys.exit(7)\n")
            command.chmod(0o755)
            env = dict(os.environ, PATH=tmp+os.pathsep+os.environ["PATH"])
            result = subprocess.run([sys.executable, str(SCRIPT), "--datacard", "datacard_{mass}.root", "--masses", "500", "600"], cwd=tmp, env=env, capture_output=True, text=True)
            self.assertEqual(result.returncode, 1)
            records = json.loads((Path(tmp) / "combine_results.json").read_text())
            self.assertEqual([r["exit_code"] for r in records], [0, 7])
            self.assertNotIn("expected", records[1])
            self.assertIn("fixture failure", (Path(tmp) / "result.txt").read_text())
            self.assertEqual(len((Path(tmp) / "higgsLimits.txt").read_text().splitlines()), 2)
            for record in records:
                argv = record["command"]
                self.assertEqual(argv[argv.index("-m")+1], str(record["mass"]))
                self.assertIn("blind", argv)


if __name__ == "__main__":
    unittest.main()
