% setPlotParameters - This function stores general parameters for figure
% generation
%
% Inputs:
%   outputFolderName - [String] Folder name where the figures should be
%   stored
%
% Outputs:
%   PlotParams - [Struct] Contains general figure parameters
%
% Date: 07.08.2018

function  PlotParams  = setPlotParameters( outputFolderName )

% Figure defaults
PlotParams.figure = {'position',  [40 70 1100 750];
                    'name',           'VMPS Evaluation Tool';
                    'numbertitle',    'off';
                    'color',          'white'; 
                    'renderer',       'opengl'; }';

PlotParams.axes = { 'box',       'on';
                    'XGrid',     'on';
                    'YGrid',     'on';
                    'XMinorGrid','off'; }';
                
PlotParams.fonts = {'FontSize',    12;
                    'FontWeight',  'bold'
                    'FontAngle',   'normal';
                    'FontName',    'Helvetica';}';
                
PlotParams.tickfonts = {'FontSize',        13;
                       'FontWeight',      'bold'}';
                   
PlotParams.titlefonts = {'FontSize',       14;
                        'FontWeight',     'bold'}';
                    
PlotParams.line = {'LineStyle',     '-';
                   'LineWidth',     1; 
                   'Marker',       'none';}';
               
% line defaults
PlotParams.lineDefaults = {'Marker','none'; % '+' | 'o' | '*' | '.' | 'x' | 'square' | 'diamond' | 'v' | '^' | '>' | '<' | 'pentagram' | 'hexagram' | 'none'
                }';               
 
PlotParams.PaperUnits = 'inches';

% Color settings
% Color map is seted to 'parula'
 PlotParams.cm =   {'parula','jet','spring','summer','autumn','winter',...
                    'gray','bone','copper','pink','lines','colorcube',...
                    'prism','flag','white'};
                
PlotParams.cmFcn = str2func(PlotParams.cm{1});

% Define format
PlotParams.format = 'png';

% Childflip
PlotParams.childflip = 1;

% Combined plots
PlotParams.combined = {'units','normalized','outerposition',[0 0 1 1]};
PlotParams.combinedPaperPosition = [0 0 12 12];

% Define directory
PlotParams.dir = outputFolderName;

% Define maximum number of xticks
PlotParams.MaxNumberXTick = 15;

end