
close all

mean1 = 0;
std1 = 1;
mean2 = 1;
std2 = 0.5;

minMean = min(mean1,mean2);
maxMean = max(mean1,mean2);

minStd = min(std1,std2);
maxStd = max(std1,std2);


allInOnefig = figure();
figure(allInOnefig);
x1 = [-mean1-5*std1:.01:mean1+5*std1];
x2 = [-mean2-5*std2:.01:mean2+5*std2];
gauss1 = normpdf(x1,mean1,std1);
gauss2 = normpdf(x2,mean2,std2);

joint = normpdf(x2,mean2,sqrt((std1^2 * std2^2)/(std1^2 + std2^2)));

Hypothesis = @(x) normpdf(x,mean1,std1)/(normpdf(x,mean1,std1)+normpdf(x,mean2,std2));

%normpdf(0,0,1)/(normpdf(0,0,1)+normpdf(0,0,4))

HypothesisProbability = arrayfun(Hypothesis,x2);

hold on
plot(x1,-gauss1,'LineWidth',2)
plot(x2,-gauss2,'LineWidth',2)
plot(x2,HypothesisProbability,'LineWidth',2)

xlim([min(mean1-5*std1,mean2-5*std2), mean2+5*std2])


xlabel('Z')
ylabel('Probability')


title('Hypothesis testing')
legend(['H_0 - [\mu_1=' num2str(mean1) ',\sigma_1=' num2str(std1) ']'],['H_1 - [\mu_2=' num2str(mean2) ',\sigma_2=' num2str(std2) ']'], ['P(H_0|Z=z)'])


