close all

mean1 = 15;
std1 = 80;
mean2 = 21;
std2 = 80;

minMean = min(mean1,mean2);
maxMean = max(mean1,mean2);

minStd = min(std1,std2);
maxStd = max(std1,std2);


allInOnefig = figure();
figure(allInOnefig);
x = [-minMean-5*maxStd:.01:maxMean+5*maxStd];
gauss1 = normpdf(x,mean1,std1);
gauss2 = normpdf(x,mean2,std2);

hold on
plot(x,gauss1,'LineWidth',3)
plot(x,gauss2,'LineWidth',3)


std12 = sqrt((std1^2*std2^2)/(std1^2+std2^2));
mean12 = (mean1*std2^2 + mean2*std1^2)/(std1^2+std2^2);

jointProbability = normpdf(x,mean12,std12);
%plot(x,jointProbability)


% assumption. first gauss is right. What is the probability we get a mean2
% measurement if the measurment's std is std2

meanP = mean1;
stdP = sqrt(std1^2+std2^2);
PMean2 = normpdf(x,meanP,stdP);

plot(x,PMean2,'LineWidth',3)

title('Normal pdfs')
legend('Measurement A - [x_1,\sigma_1^2]','Measurement B - [x_2,\sigma_2^2]','P of Measurement B gets')
legend(['Measurement A - [\mu_1=' num2str(mean1) ',\sigma_1=' num2str(std1) ']'],['Measurement B - [\mu_2=' num2str(mean2) ',\sigma_2=' num2str(std2) ']'], ['P(B:z|A)'])

xlim([min(mean1-4*std1,mean2-4*std2), mean2+4*std2])


xlabel('Z')
ylabel('Probability')


title('Probability of Measurement B if Measurement A is unbiased')

normpdf(mean2,mean1,stdP)
p1 = cdf('Normal',mean2-1,mean1,stdP);
p2 = cdf('Normal',mean2+1,mean1,stdP);

p2-p1

hold on


