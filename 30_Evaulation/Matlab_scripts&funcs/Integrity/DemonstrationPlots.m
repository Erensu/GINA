close all

mean1 = 15;
std1 = 1;
mean2 = 21;
std2 = 4;

minMean = min(mean1,mean2);
maxMean = max(mean1,mean2);

minStd = min(std1,std2);
maxStd = max(std1,std2);


allInOnefig = figure();
figure(allInOnefig);
x = [-minMean-4*maxStd:.1:maxMean+4*maxStd];
gauss1 = normpdf(x,mean1,std1);
gauss2 = normpdf(x,mean2,std2);

hold on
plot(x,-gauss1,'LineWidth',3)
plot(x,-gauss2,'LineWidth',3)


std12 = sqrt((std1^2*std2^2)/(std1^2+std2^2));
mean12 = (mean1*std2^2 + mean2*std1^2)/(std1^2+std2^2);

jointProbability = normpdf(x,mean12,std12);
plot(x,jointProbability,'LineWidth',3)

title('Normal pdfs')
legend('Normal Distribution 1','Normal Distribution 2', 'jointProbability')

%cdfFigure = figure();
%figure(cdfFigure);


mu = meanP;
sigma = stdP;


hold on

%plot(x,y2)
%plot(x,PMean2)
%title('Normal cdf')
%legend('CDF of Normal Distribution')





normpdf(mean2,mean1,stdP)
title('Normal pdfs')
%legend('Measurement A - [x_1,\sigma_1^2]','Measurement B - [x_2,\sigma_2^2]','P of Measurement B gets')
%legend(['Measurement A - [\mu_1=' num2str(mean1) ',\sigma_1=' num2str(std1) ']'],['Measurement B - [\mu_2=' num2str(mean2) ',\sigma_2=' num2str(std2) ']'], ['Estimation - [\mu_{12}=' num2str(mean12) ',\sigma_{12}=' num2str(std12) ']'])
legend(['Measurement A - [\mu_1=' num2str(mean1) ',\sigma_1=' num2str(std1) ']'],['Measurement B - [\mu_2=' num2str(mean2) ',\sigma_2=' num2str(std2) ']'], ['Model C - \delta(x-', num2str(mean12),')'])

xlim([min(mean1-4*std1,mean2-4*std2), mean2+4*std2])


xlabel('Z')
ylabel('Probability')


title('Measurement A & B and the Priori Model')



