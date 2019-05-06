function [mu, min_mu, max_mu, sigma, min_sigma, max_sigma, M, Std, Min, Max, Median] = MakeStatisticsPlot(Path, Prefix, PostFix, Map , fitGauss, confidanceInterval )

f = figure();
mu=0; min_mu=0; max_mu=0; sigma=0; min_sigma=0; max_sigma=0;

values = Map(:,3);
%h = histogram(values);
%h.NumBins = numberOfBins;

M = mean(values);
Std = std(values);
Min = min(values);
Max = max(values);
Median = median(values);

hist(values);

if(fitGauss)
    try
        [pd] = fitdist(values,'Normal');

        pd_ci = paramci(pd,confidanceInterval);

        mu = pd.mu;
        min_mu = pd_ci(1,1);
        max_mu = pd_ci(2,1);
        sigma = pd.sigma;
        min_sigma = pd_ci(1,2);
        max_sigma = pd_ci(2,2);
    catch
    end
end
% pd.mean
% pd_ci(1,1)
% pd_ci(2,1)
% pd.mean - pd_ci(1,1)
% pd_ci(2,1) - pd.mean
% 
% pd.sigma
% pd_ci(1,2)
% pd_ci(2,2)
% pd.sigma - pd_ci(1,2)
% pd_ci(2,2) - pd.sigma

%str = {{['\mu=' num2str(pd.mean),' \sigma=' num2str(pd.sigma) ]},{['99.73% confidence interval of \mu=' num2str(pd_ci(1,1)),' - ',num2str(pd_ci(2,1))]},{['99.73% confidence interval of \sigma=' num2str(pd_ci(1,2)),' - ',num2str(pd_ci(2,2))]} };
%text([min(values),min(values),min(values)],[length(values)/(0.5*numberOfBins),length(values)/(0.5*numberOfBins)-25, length(values)/(0.5*numberOfBins)-50],str)

savefig(f, fullfile(Path, [Prefix, 'Hist', PostFix,'.fig']));
saveas(f, fullfile(Path, [Prefix, 'Hist', PostFix,'.png']));

close(f);

end