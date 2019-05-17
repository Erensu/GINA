function [] = PlotIONEXdiff(plot_file, outFolder, PrefixforGrids, PrefixAll, PostFixAll, OnlyAllDataHist, fitGauss, IntervalProbability, binNumberOfAllHist)

    filewPath = which(plot_file);
    if exist(filewPath, 'file') ~= 2
        error('%s file open error\n', filewPath);
    end
    [filepath,name,ext] = fileparts(filewPath);

    %[~,name_out,ext_out] = fileparts(sorted_plot_file);
    %outFilewPath = fullfile(filepath,[name_out,'.mat']);

    % Import data to matrix form
    A = importdata(filewPath);
    
    if(isempty(A))
       sprintf('%s file import error\n', filewPath); 
       return;
    end

    % Sort matrix by lat lon rows
    D = sortrows(A,[1 2]);

    % Save sorted file
    % save(outFilewPath,'D');

    RefRow = 1;

if(OnlyAllDataHist ~= 1)
    
    if fitGauss == 1
        fileID = fopen(fullfile(outFolder, [PrefixforGrids, '_Fitted_GaussianParameters_gridwise','.txt']),'w');
    end
    
    fileID_SimpleStat = fopen(fullfile(outFolder, [PrefixforGrids, '_SimpleStat_','.txt']),'w');
    
    while(RefRow<length(D))
    [~, index]=ismember(D(:,1:2),D(RefRow,1:2),'rows');

    I = [1:1:length(D)]'.*index;
    I = find(I>0);
    Map = D(I,1:3);


    PostFixForGrid = sprintf('_%2.2f_lon%2.2f',Map(1,1), Map(1,2));
    [mu, min_mu, max_mu, sigma, min_sigma, max_sigma, M, Std, Min, Max, Median] = MakeStatisticsPlot( outFolder, PrefixforGrids, PostFixForGrid, Map, fitGauss, IntervalProbability,0 );

    if fitGauss == 1
       fprintf(fileID,'%6.2f %6.2f %6.3f %6.3f %6.3f %6.3f %6.3f %6.3f \n',Map(1,1), Map(1,2), mu, min_mu, max_mu, sigma, min_sigma, max_sigma);
    end
    
    fprintf(fileID_SimpleStat,'%6.2f %6.2f %6.3f %6.3f %6.3f %6.3f %6.3f\n',Map(1,1), Map(1,2), M, Std, Min, Max, Median);
   
    RefRow = RefRow + length(I);

    end
    
	if fitGauss == 1
        fclose(fileID);
	end
     
	fclose(fileID_SimpleStat);
    
end
  [mu, min_mu, max_mu, sigma, min_sigma, max_sigma, M, Std, Min, Max, Median] =  MakeStatisticsPlot( outFolder, PrefixAll, PostFixAll, D, fitGauss, IntervalProbability, binNumberOfAllHist );
%% Save FittedParameters
    if fitGauss == 1
        fileID = fopen(fullfile(outFolder, [PrefixAll, '_Fitted_GaussianParameters_', PostFixAll,'.txt']),'w');
        fprintf(fileID,'%6.4f %6.4f %6.4f %6.4f %6.4f %6.4f',mu, min_mu, max_mu, sigma, min_sigma, max_sigma);
        fclose(fileID);
    end
    
    fileID = fopen(fullfile(outFolder, [PrefixAll, '_SimpleStat_', PostFixAll,'.txt']),'w');
     fprintf(fileID,'%6.3f %6.3f %6.3f %6.3f %6.3f\n', M, Std, Min, Max, Median);
     fclose(fileID);
   
end


