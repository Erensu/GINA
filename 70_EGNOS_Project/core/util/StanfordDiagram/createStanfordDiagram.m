% createStanfordDiagram - This function plots the stanford diagram
%
% Inputs:
%   xAxesData: data in horizontal axes, typically the estimated error

%   yAxesData: data in vertical axes, typically the estimated error

%   pointsize: point size, this input is not relevant in case the standard
%   stanford diagram is plotted

%   colorData: can be empty [], which orders the function to plot the
%   standard stanford diagram (frequency as color). 

%   AlertLimit: Alert limit in the stanford diagram

%   strTitle: the title of the figure

%   strColorylabel: lable for the colorbar
%
%   strOutputPath: path including file name for the 

%
% Created by: Mohammad Tourian
% Date: 09.10.2017
% Date: 11.09.2018

function  createStanfordDiagram(xAxesData,yAxesData,pointsize,colorData,AlertLimit,strTitle,strColorylabel,strOutputPath, pparam)

fig = figure;

set(fig,pparam.figure{:});
fh = gcf;
fh.Visible = 'off';
% defining the maximum extent of x and y axes
maxX=max([xAxesData ;max(AlertLimit)])+2;

maxY=max([yAxesData ;max(AlertLimit)])+2;

maxXY=max([maxX maxY]);


% if the user provides the colorData, the stanford diagram plots x,y,
% colorData in color
if isempty(colorData)==0
    
    scatter(xAxesData,yAxesData,pointsize,colorData,'filled')

else
    NrOfGrids=100;
    xdata=linspace(0,maxXY,NrOfGrids); 
    ydata=linspace(0,maxXY,NrOfGrids);
    
    for i=2:length(xdata)
        for j=2:length(ydata)
            f=find(xAxesData>xdata(i-1) & xAxesData<=xdata(i) &yAxesData>ydata(j-1) & yAxesData<=ydata(j));
            cdata(j-1,i-1)=length(f);
        end
    end
    
    cdata=[zeros(NrOfGrids-1,1) cdata;zeros(1,NrOfGrids)];
    % turning the cdata into the frequency [%]
    cdata=100*cdata./sum(cdata(:));
    
    imagesc(xdata,ydata,cdata,[0 max(cdata(:))]);
    %
    c=parula(256);
    
    % making the first color value white
    c(1,:)=[1 1 1];
    
    % applying the colormap
    colormap(c);
    
    set(gca,'YDir','normal')
    
end
    
pbaspect([1,1,1])

xlim([0 AlertLimit+2])
ylim([0 AlertLimit+2])

for i=1:length(AlertLimit)
    
    line([0 maxXY],[AlertLimit(i) AlertLimit(i)],'linewidth',2,'color','k')
    line([AlertLimit(i) AlertLimit(i)],[0 maxXY],'linewidth',2,'color','k')
    
end

line([0 maxXY],[0 maxXY],'linewidth',2,'color','k')

% Set figure title and labels
title(['Stanford Diagram ' strTitle])
xlabel('Error [m]')
ylabel('Protection Level [m]')

% Set font size and style
set(gca,pparam.fonts{:})

hColorBar=colorbar;
ylabel(hColorBar,strColorylabel)

%% save
    actformat = pparam.format;
    %print(strOutputPath,['-d',actformat],'-r300');
    saveas(gcf,[strOutputPath '.' actformat]);     %by zge2bp
    
    %set the visibility of the figure to 'on' before saving 
    set(fig, 'CreateFcn', 'set(gcbo,''Visible'',''on'')'); 
    
    savefig(fig, strOutputPath);


end

