
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
%x1 = [-mean1-5*std1:.01:mean1+5*std1];
x = [-mean2-5*std2:.01:mean2+5*std2];
gauss1 = normpdf(x,mean1,std1);
gauss2 = normpdf(x,mean2,std2);

Hypothesis_Btrue = @(mean1,mean2,std1,std2) (1/(sqrt(2*pi).*std2))./((1/(sqrt(2*pi).*std2))+normpdf(mean1,mean2,sqrt(std1.^2 + std2.^2)));
Hypothesis_Atrue = @(mean1,mean2,std1,std2) (1/(sqrt(2*pi).*std1))./((1/(sqrt(2*pi).*std1))+normpdf(mean1,mean2,sqrt(std1.^2 + std2.^2)));

Hypothesis_Btrue(mean1,mean2,std1,std2)
Hypothesis_Atrue(mean1,mean2,std1,std2)
%normpdf(X,mean2,sqrt(std1^2 + std2^2))
%((1/(sqrt(2*pi)*std2))+normpdf(X,mean2,sqrt(std1^2 + std2^2)))
%(1/(sqrt(2*pi)*std2))./((1/(sqrt(2*pi)*std2))+normpdf(X,mean2,sqrt(std1^2 + std2^2)))
%normpdf(0,0,1)/(normpdf(0,0,1)+normpdf(0,0,4))
%h = zeros(10,10);

%[X,Y] = meshgrid(0:0.1:8.9);
%deltaX = [0:0.1:10];
%std = [0:0.1:10];
%for i=1:90
%    for j=1:90
%        
%        h(i,j) = Hypothesis(mean1,deltaX(i),std1,std(j));
%    end
%end

%HypothesisProbability = arrayfun(Hypothesis,x,1,2,3);

%hold on
%plot(x,-gauss1,'LineWidth',2)
%plot(x,-gauss2,'LineWidth',2)
%plot(x,HypothesisProbability,'LineWidth',2)

%xlim([min(mean1-5*std1,mean2-5*std2), mean2+5*std2])

%surf(X,Y,h)

%xlabel('Z')
%ylabel('Probability')


%title('Hypothesis testing')
%legend(['H_0 - [\mu_1=' num2str(mean1) ',\sigma_1=' num2str(std1) ']'],['H_1 - [\mu_2=' num2str(mean2) ',\sigma_2=' num2str(std2) ']'], ['P(H_0|Z=z)'])


