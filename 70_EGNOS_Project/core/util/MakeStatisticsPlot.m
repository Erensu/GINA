function MakeStatisticsPlot(Path, Prefix, PostFix, Map , numberOfBins )

f = figure();

values = Map(:,3);
h = histogram(values);
h.NumBins = numberOfBins;

savefig(f, fullfile(Path, [Prefix, 'Hist', PostFix,'.fig']));
saveas(f, fullfile(Path, [Prefix, 'Hist', PostFix,'.png']));

close(f);

end