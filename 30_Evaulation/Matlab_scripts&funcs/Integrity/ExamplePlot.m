
close all

mean1 = 0;
std1 = 1;
mean2 = 0;
std2 = 1;

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


xlabel('TEC')
ylabel('probability density')


title('Measurement A & B')
legend('Measurement A - [x_1,\sigma_1^2]','Measurement B - [x_2,\sigma_2^2]')


