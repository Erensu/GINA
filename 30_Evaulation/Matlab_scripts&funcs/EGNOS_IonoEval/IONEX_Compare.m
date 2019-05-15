function IONEX_Compare(input_File, out_dataFolder, out_picsFolder)
%Matlab fcn to read ionex file and draw Total Electron Content map(s)

    %folder = fileparts(which(mfilename)); 
    %addpath(genpath(folder));

%     ionex_file = '\DebugFiles\Diff_CODG190-y2019_d019_136.ion';  %just for testing
%     plusFilePath2Pics = '\DebugPlots';
%     plusFilePath2POutput = '\DebugOutputs';
% 
%     filewPath = which(ionex_file);
    
    if exist(input_File, 'file') ~= 2
        error('%s file open error\n', input_File);
    end
    
    [filepath,name,ext] = fileparts(input_File);

    outPutFileNamewPath_TEC = ['PlotData_',name,'_TEC','.ionTEC'];
    outPutFileNamewPath_RMS = ['PlotData_',name,'_RMS','.ionRMS'];
    outPutFileNamewPath_MLH = ['PlotData_',name,'_MLH','.ionMLH'];
    outPutFileNamewPath_CH2 = ['PlotData_',name,'_CH2','.ionCH2'];

    fullFileName_TEC = fullfile(out_dataFolder,outPutFileNamewPath_TEC);
    fullFileName_RMS = fullfile(out_dataFolder,outPutFileNamewPath_RMS);
    fullFileName_MLH = fullfile(out_dataFolder,outPutFileNamewPath_MLH);
    fullFileName_CH2 = fullfile(out_dataFolder,outPutFileNamewPath_CH2);

    fopen(fullFileName_TEC,'w');
    fopen(fullFileName_RMS,'w');
    fopen(fullFileName_MLH,'w');
    fopen(fullFileName_CH2,'w');
    close all;

    [fin, errormsg] = fopen(input_File, 'r');
    if errormsg
        error('%s file open error\n', input_File);
        exit
    end

    %map of the world
    world_map = load('coastline.txt');

    n_maps=0; n_rms=0; n_mlh = 0; n_ch2 = 0;
    while ~feof(fin)
        tline = fgetl(fin);
        if strfind(tline, 'START OF DATA')
          HPL = str2num(tline(4:10));
        end
         if strfind(tline, 'HPL')
          HPL = str2num(tline(4:10));
         end
        if strfind(tline, 'LAT1 / LAT2 / DLAT')
          lat1 = str2num(tline(2:8));
          lat2 = str2num(tline(9:15));
          dlat = str2num(tline(16:21));
          lat = lat1:dlat:lat2;
        end
        if strfind(tline, 'LON1 / LON2 / DLON')
          lon1 = str2num(tline(2:8));
          lon2 = str2num(tline(9:15));
          dlon = str2num(tline(16:21));
          lon = lon1:dlon:lon2;
          %o = size(lon, 2);  %number of values in a block
          %n = fix(o / 16) + 1;            %number of rows in a block
        end


        if (~isempty(strfind(tline, 'START OF TEC MAP')) || ~isempty(strfind(tline, 'START OF RMS MAP'))|| ~isempty(strfind(tline, 'START OF MLH MAP'))|| ~isempty(strfind(tline, 'START OF CH2 MAP')))
        %todo: check lat, lon vectors exist
            type_of_data = tline(70:73);
            if strfind(type_of_data,'TEC')
                unitOfData = 0.1;
                n_maps=n_maps+1;                           %index of current map
            elseif strfind(type_of_data,'RMS')
                unitOfData = 0.1;
                n_rms=n_rms+1;
            elseif strfind(type_of_data,'MLH')
                unitOfData = 0.0001;
                n_mlh=n_mlh+1;
            elseif strfind(type_of_data,'CH2')
                unitOfData = 0.0001;
                n_ch2=n_ch2+1;
            end  

            %% Read data block
            [iono_map, year, month, day, hour, minute, sec] = readBlock(fin, lat, lon, unitOfData);

            if(isempty(iono_map))
                continue;
            end

            %% Short output of current map
            if strfind(type_of_data,'TEC')
                fprintf ('%d TEC MAP read %d %02d %02d %02d:%02d:%02d\n', n_maps, year, month, day, hour, minute, sec);
            elseif strfind(type_of_data,'RMS')
                fprintf ('%d RMS MAP read %d %02d %02d %02d:%02d:%02d\n', n_rms, year, month, day, hour, minute, sec);
            elseif strfind(type_of_data,'MLH')
                fprintf ('%d MLH MAP read %d %02d %02d %02d:%02d:%02d\n', n_mlh, year, month, day, hour, minute, sec);
            elseif strfind(type_of_data,'CH2')
                fprintf ('%d CH2 MAP read %d %02d %02d %02d:%02d:%02d\n', n_ch2, year, month, day, hour, minute, sec);
            end 

            fprintf ('number of data %d\n', size(iono_map, 1));
            fprintf ('max %.1f min %.1f\n', max(iono_map(:,3)), min(iono_map(:,3)));


            %% Create plot file name and plot title
            fullFileName = [];
            plotTitle = [];
            if strfind(type_of_data,'TEC')
                plotTitle = ['Total Electron Content Map %d %02d %02d %02d:%02d:%02d', year, month, day, hour, minute, sec];
                fullFileName = fullfile(out_picsFolder,sprintf(strcat(strrep(strcat(name,ext),'.','_'), '_iono%02d'), n_maps));
            elseif strfind(type_of_data,'RMS')
                plotTitle = ['Total Electron Content RMS Map %d %02d %02d %02d:%02d:%02d', year, month, day, hour, minute, sec];
                fullFileName = fullfile(out_picsFolder,sprintf(strcat(strrep(strcat(name,ext),'.','_'), '_rms%02d'), n_rms));
            elseif strfind(type_of_data,'MLH')
                plotTitle = ['Total Electron Content MLH Map %d %02d %02d %02d:%02d:%02d', year, month, day, hour, minute, sec];
                fullFileName = fullfile(out_picsFolder,sprintf(strcat(strrep(strcat(name,ext),'.','_'), '_mlh%02d'), n_rms));

            elseif strfind(type_of_data,'CH2')
                plotTitle = ['Total Electron Content CH2 Map %d %02d %02d %02d:%02d:%02d', year, month, day, hour, minute, sec];
                fullFileName = fullfile(out_picsFolder,sprintf(strcat(strrep(strcat(name,ext),'.','_'), '_ch2%02d'), n_rms));
            end

            %% Plot
            % Calc min max values for plot limit
            minData = min(iono_map(:,3));
            maxData = max(iono_map(:,3));
            delta = (maxData-minData)/20;
            if(abs(delta) > 0.00001 )
                 minC = minData - delta;
                 maxC = maxData + delta;

            else
                if(abs(minData) < 0.00001 && abs(maxData) < 0.00001)
                     minC = 0;
                     maxC = 1;
                else
                    minC = minData/2;
                    maxC = 2*maxData;
                end
            end

            plotIonoMap(fullFileName, world_map, iono_map, plotTitle, minC, maxC);

            %% Save Iono Map data to File
            fullFileName = [];
            if strfind(type_of_data,'TEC')
                fullFileName_TEC = fullfile(out_dataFolder,outPutFileNamewPath_TEC);
                fileID_TEC = fopen(fullFileName_TEC,'a');
                fprintf(fileID_TEC,'%6.2f %6.2f %12.8f\n',iono_map');
                fclose(fileID_TEC);
            elseif strfind(type_of_data,'RMS')
                fullFileName_RMS = fullfile(out_dataFolder,outPutFileNamewPath_RMS);
                fileID_RMS = fopen(fullFileName_RMS,'a');
                fprintf(fileID_RMS,'%6.2f %6.2f %12.8f\n',iono_map');
                fclose(fileID_RMS);
            elseif strfind(type_of_data,'MLH')
                fullFileName_MLH = fullfile(out_dataFolder,outPutFileNamewPath_MLH);
                fileID_MLH = fopen(fullFileName_MLH,'a');
                fprintf(fileID_MLH,'%6.2f %6.2f %12.8f\n',iono_map');
                fclose(fileID_MLH);
            elseif strfind(type_of_data,'CH2')
                fullFileName_CH2 = fullfile(out_dataFolder,outPutFileNamewPath_CH2);
                fileID_CH2 = fopen(fullFileName_CH2,'a');
                fprintf(fileID_CH2,'%6.2f %6.2f %12.8f\n',iono_map');
                fclose(fileID_CH2);
            end

        end
    end

    fclose(fin);

end