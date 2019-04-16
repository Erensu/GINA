function [ output_args ] = NormalPlots( mean1, std1, mean2, std2 )
%NORMALPLOTS Summary of this function goes here
%   Detailed explanation goes here

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
plot(x,gauss1)
plot(x,gauss2)


std12 = sqrt((std1^2*std2^2)/(std1^2+std2^2));
mean12 = (mean1*std2^2 + mean2*std1^2)/(std1^2+std2^2);

jointProbability = normpdf(x,mean12,std12);
plot(x,jointProbability)



% assumption. first gauss is right. What is the probability we get a mean2
% measurement if the measurment's std is std2

meanP = mean1;
stdP = sqrt(std1^2+std2^2);
PMean2 = normpdf(x,meanP,stdP);

plot(x,PMean2)

title('Normal pdfs')
legend('Normal Distribution 1','Normal Distribution 2', 'jointProbability', 'Pmean2')

%cdfFigure = figure();
%figure(cdfFigure);


mu = meanP;
sigma = stdP;

y2 = cdf('Normal',x,mu,sigma);

hold on

%plot(x,y2)
%plot(x,PMean2)
%title('Normal cdf')
%legend('CDF of Normal Distribution')

cdf('Normal',mean2,mu,sigma)

mean2;
mu;
sigma;


p1 = cdf('Normal',mean2,mu,sigma)
p2 = cdf('Normal',mean2,mu,sigma)

mean1
mean2
meanP
stdP

normpdf(mean2,mean1,stdP)

p2-p1;

y1 = normpdf(x,1,1);
y2 = normpdf(x,1,0.5);
norm(y1.*y2)

end

