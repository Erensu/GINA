function o2mDir(dirIn)
% function o2mDir(dirIn)
%  converts all of the m-files in directory dirIn to Matlab 
%  compatible m-files.

tic
convDir=dir(dirIn);
for ii=1:length(convDir)
 if ~isempty(convDir(ii).name)
   if length(convDir(ii).name)>2
    if strcmp(convDir(ii).name(end-1:end),'.m')
     eval(['oct2ml(''',dirIn,filesep,convDir(ii).name,''');'])
    end % if strcmp(convDir(ii).
   end % if length(convDir(ii).
 end % if ~isempty(convDir(ii).
end % for ii=1:length(convDir)
%delete([dirIn,filesep,'*.PREo2m'])
toc