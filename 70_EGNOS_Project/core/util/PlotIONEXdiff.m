function [] = PlotIONEXdiff(plot_file, sorted_plot_file, extraPlotSubFolder, PrefixforGrids, PrefixAll, PostFixAll, OnlyAllDataHist, gridBinNumber, AllDataBinNumber)

    filewPath = which(plot_file);
    if exist(filewPath, 'file') ~= 2
        error('%s file open error\n', filewPath);
    end
    [filepath,name,ext] = fileparts(filewPath);

    [~,name_out,ext_out] = fileparts(sorted_plot_file);
    outFilewPath = fullfile(filepath,[name_out,'.mat']);

    % Import data to matrix form
    A = importdata(filewPath);

    if(isempty(A))
       error('%s file import error\n', filewPath); 
    end

    % Sort matrix by lat lon rows
    D = sortrows(A,[1 2]);

    % Save sorted file
    save(outFilewPath,'D');

    RefRow = 1;

if(OnlyAllDataHist ~= 1)
    while(RefRow<length(D))
    [~, index]=ismember(D(:,1:2),D(RefRow,1:2),'rows');

    I = [1:1:length(D)]'.*index;
    I = find(I>0);
    Map = D(I,1:3);


    PostFixForGrid = sprintf('_%2.2f_lon%2.2f',Map(1,1), Map(1,2));
    MakeStatisticsPlot( fullfile(filepath,extraPlotSubFolder), PrefixforGrids, PostFixForGrid, Map, gridBinNumber );

    RefRow = RefRow + length(I);

    end
end

    MakeStatisticsPlot( fullfile(filepath,extraPlotSubFolder), PrefixAll, PostFixAll, D, AllDataBinNumber );


end


