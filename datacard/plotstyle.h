void applycanvasstyle(TCanvas *c){
    c->SetFillColor(0);
    c->SetBorderMode(0);
    c->SetBorderSize(2);
    c->SetTickx(1);
    c->SetTicky(1);
    c->SetLeftMargin(0.17);
    c->SetRightMargin(0.05);
    c->SetTopMargin(0.07);
    c->SetBottomMargin(0.13);
    c->SetFrameFillStyle(0);
    c->SetFrameBorderMode(0);
    c->SetFrameFillStyle(0);
    c->SetFrameBorderMode(0);
}
void applylegendstyle(TLegend *l2D){
    l2D->SetBorderSize(0);
    l2D->SetTextFont(42);
    l2D->SetTextSize(0.04);
    l2D->SetLineColor(1);
    l2D->SetLineStyle(1);
    l2D->SetLineWidth(1);
    l2D->SetFillColor(0);
    l2D->SetFillStyle(0);
}
void applyaxesstyle(TH1 *h){
    h->GetXaxis()->SetNdivisions(505);
    h->GetXaxis()->SetLabelFont(42);
    h->GetXaxis()->SetLabelOffset(0.007);
    h->GetXaxis()->SetLabelSize(0.04);
    h->GetXaxis()->SetTitleSize(0.06);
    h->GetXaxis()->SetTitleOffset(0.9);
    h->GetXaxis()->SetTitleFont(42);
    h->GetYaxis()->SetNdivisions(505);
    h->GetYaxis()->SetLabelFont(42);
    h->GetYaxis()->SetLabelOffset(0.007);
    h->GetYaxis()->SetLabelSize(0.04);
    h->GetYaxis()->SetTitleSize(0.06);
    h->GetYaxis()->SetTitleOffset(1.1);
    h->GetYaxis()->SetTitleFont(42);
    h->GetXaxis()->CenterTitle();
    h->GetYaxis()->CenterTitle();
}

