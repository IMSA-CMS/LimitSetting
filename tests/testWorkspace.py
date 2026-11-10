import json
import math
from pathlib import Path
import sys
from array import array
import ROOT

ROOT.gROOT.SetBatch(True)
mode = sys.argv[1]
if mode == 'write':
    assert Path.cwd().name == 'software-fixture'
    f = ROOT.TFile('fixture.root', 'RECREATE')
    for name, bins in [('0tau_signal', [1, 2, 3, 4]), ('0tau_ZZ', [4, 3, 2, 1]), ('0tau_ttbarMultiboson', [2, 4, 1, 3])]:
        h = ROOT.TH2D(name, 'Software fixture only', 2, array('d', [50, 100, 300]), 2, array('d', [50, 200, 400]))
        h.GetXaxis().SetTitle('m++')
        h.GetYaxis().SetTitle('m--')
        for ix in range(1, 3):
            for iy in range(1, 3):
                h.SetBinContent(ix, iy, bins[2*(ix-1)+iy-1])
        h.Write()
    f.Close()
    assert ROOT.gROOT.LoadMacro(str(Path(sys.argv[2]).resolve())+'+') == 0
    ROOT.prepareForLimit(False, 'fixture.root', 500, '0tau')
    assert not Path('datacard_syst.txt').exists()
    try:
        ROOT.prepareForLimit(True, 'fixture.root', 500, '0tau')
    except Exception:
        pass
    else:
        raise AssertionError('Systematic model was accepted without joint systematic inputs')
    rejected = []
    for defect in ['missing', 'axis', 'negative', 'nonfinite', 'flow', 'cancelled_flow', 'binning']:
        source = ROOT.TFile.Open('fixture.root')
        bad = ROOT.TFile('invalid_'+defect+'.root', 'RECREATE')
        for name in ['0tau_signal', '0tau_ZZ', '0tau_ttbarMultiboson']:
            if defect == 'missing' and name == '0tau_signal':
                continue
            h = source.Get(name).Clone()
            h.SetDirectory(bad)
            if name == '0tau_signal':
                if defect == 'axis': h.GetXaxis().SetTitle('unknown')
                if defect == 'negative': h.SetBinContent(1, 1, -1)
                if defect == 'nonfinite': h.SetBinContent(1, 1, float('nan'))
                if defect == 'flow': h.SetBinContent(0, 1, 1)
                if defect == 'cancelled_flow': h.SetBinError(0, 1, 1)
                if defect == 'binning': h.GetXaxis().Set(2, array('d', [50, 110, 300]))
            h.Write()
        bad.Close()
        source.Close()
        try:
            ROOT.prepareForLimit(False, 'invalid_'+defect+'.root', 500, '0tau')
        except Exception:
            rejected.append(defect)
        else:
            raise AssertionError('Invalid input accepted: '+defect)
    print(json.dumps({'invalid_templates_rejected': rejected}))
elif mode == 'check':
    f = ROOT.TFile('higgsworkspace.root')
    w = f.Get('higgsworkspace')
    x, y = w.var('massX'), w.var('massY')
    variables = ROOT.RooArgSet(x, y)
    for name, bins in [('0tau_signal', [1, 2, 3, 4]), ('0tau_ZZ', [4, 3, 2, 1]), ('0tau_ttbarMultiboson', [2, 4, 1, 3])]:
        pdf = w.pdf(name)
        assert pdf and pdf.ClassName() == 'RooHistPdf'
        assert math.isclose(pdf.createIntegral(variables, ROOT.RooFit.NormSet(variables)).getVal(), 1, rel_tol=1e-12)
        assert math.isclose(w.var(name+'_norm').getVal(), sum(bins), rel_tol=1e-12)
        recovered = []
        for ix, (lo, hi) in enumerate([(50, 100), (100, 300)]):
            for iy, (yl, yh) in enumerate([(50, 200), (200, 400)]):
                x.setVal((lo+hi)/2)
                y.setVal((yl+yh)/2)
                expected = bins[2*ix+iy]/sum(bins)/(hi-lo)/(yh-yl)
                assert math.isclose(pdf.getVal(variables), expected, rel_tol=1e-12)
                recovered.append(pdf.getVal(variables)*(hi-lo)*(yh-yl)*sum(bins))
        assert all(math.isclose(a, b, rel_tol=1e-12) for a, b in zip(recovered, bins))
    assert math.isclose(w.data('0tau_data_obs').sumEntries(), 20, rel_tol=1e-12)
    converted = ROOT.TFile('datacard.root')
    cw = converted.Get('w')
    assert cw.pdf('model_s') and cw.data('data_obs')
    assert not cw.set('nuisances') or cw.set('nuisances').getSize() == 0
    observables = cw.set('observables')
    for strength, expected_yield in [(0, 20), (1, 30), (2, 40)]:
        cw.var('r').setVal(strength)
        assert math.isclose(cw.pdf('model_s').expectedEvents(observables), expected_yield, rel_tol=1e-12)
    print(json.dumps({'scope': 'software fixture only, no physics result', 'joint_pdf_integrals': [1, 1, 1], 'yields': [10, 10, 10], 'observation': 20, 'converted_model': True}))
else:
    raise ValueError(mode)
