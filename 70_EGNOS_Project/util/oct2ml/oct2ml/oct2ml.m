function [filestr]=oct2ml(filename,varargin)
%oct2ml('filename')
% Call with the full function name, including extension.

tt1=cputime;
%Load keywords and function words.
r=char(10);
fs_good=[];
formats=cell(0,2); extwords=cell(0);

%First read the function into funstr.
[temp1,filename_funname,temp2]=fileparts(filename);
funstr=cell(1,1);
if exist(filename)==2
 fprintf(1,'  Converting file:  ');   fprintf(1,[filename,r]);
 fid=fopen(filename); filestr=fscanf(fid,'%c'); fclose(fid);
 if ~strcmp(filestr(length(filestr)),r), filestr=[filestr,r]; end
 rets=findstr(r,filestr);
 rets=[0 rets];
 count=1;
 temp2='';
 for i=1:length(rets)-1
  tempstr=[temp2,filestr(1+rets(i):rets(i+1)-1)];
  funstr{count}=tempstr;
  count=count+1;
 end
else
 error(['I can''t find the file ',filename,'...']);
end
funstr=deblank(funstr);
funstr=funstr';
s=length(funstr);
disp(['    Number of lines:   ',num2str(s)])



%Misc tasks

%fix variables with leading _
funstr=regexprep(funstr,'(\W)_+(\w)+(\W)','$1$2$3');

[funstr,funstrwords,funstrwords_b,funstrwords_e,funstrnumbers,funstrnumbers_b,funstrnumbers_e,s,fs_good]=updatefunstr_o(funstr);

%fix the number sign
for ii=fs_good
 temp=find(funstr{ii}=='#');
 for jj=1:length(temp)
  if ~incomment_o(funstr{ii},temp(jj)) && ~inastring_o(funstr{ii},temp(jj)) && ~inaDQstring_o(funstr{ii},temp(jj))
   funstr{ii}(temp(jj))='%';
  end % if ~incomment_o(funstr{ii},
 end % for jj=length(temp):-1:1
end
%funstr=regexprep(funstr,'#','%',1);

%fix the ! to be ~
for ii=fs_good
 temp=find(funstr{ii}=='!');
 for jj=length(temp):-1:1
  if ~incomment_o(funstr{ii},temp(jj)) && ~inastring_o(funstr{ii},temp(jj)) && ~inaDQstring_o(funstr{ii},temp(jj))
   funstr{ii}(temp(jj))='~';
  end % if ~incomment_o(funstr{ii},
 end % for jj=length(temp):-1:1
end

%fix the ** to be ^
for ii=fs_good
 temp=strfind(funstr{ii},'**');
 for jj=length(temp):-1:1
  if ~incomment_o(funstr{ii},temp(jj)) && ~inastring_o(funstr{ii},temp(jj)) && ~inaDQstring_o(funstr{ii},temp(jj))
   funstr{ii}(temp(jj))='~';
   funstr{ii}=[funstr{ii}(1:temp(jj)-1),'^',funstr{ii}(temp(jj)+2)];
  end % if ~incomment_o(funstr{ii},
 end % for jj=length(temp):-1:1
end

% fix \ (or unbalanced parentheses) continuation to be ...
for ii=fs_good
 if ~isempty(funstr{ii})
  if funstr{ii}(end)== '\'
   funstr{ii}=[funstr{ii}(1:end-1),'...'];
  end % if funstr{ii}(end)=='\'
  [outflag,howmany2,subscripts2,centercomma2,parens2]=inwhichlast_o(ii,length(funstr{ii}),funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e,filename_funname);
  if outflag
   if ~strcmp(funstr{ii}(end-2:end),'...')
   funstr{ii}=[funstr{ii},' ...'];
   end % if ~strcmp(funstr{ii}(end-2:end),
  end
 end % if ~isempty(funstr{ii})
end
%'reeeeeeee8888888899',funstr{ii},keyboard


%Change calls to octave functions with no matlab equivalent
noChangeWords={}.';  temp3=0;
for i=fliplr(fs_good)
 for j=length(funstrwords{i}):-1:1
  if ~inaDQstring_o(funstr{i},funstrwords_b{i}(j))
   if ~inastring_o(funstr{i},funstrwords_b{i}(j))
    if ~incomment_o(funstr{i},funstrwords_b{i}(j))
     tempflag=0;
     if ~any(strcmp(funstrwords{i}{j},noChangeWords))
      tempflag=0;
      tempstr=funstr{i};
     [funstr,tempflag,temp2]=wordconverter_o(i,j,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e,formats);
     if temp2, temp3=1; end
     end
     if tempflag==0
      if ~any(strcmp(funstrwords{i}{j},noChangeWords)) 
       noChangeWords{length(noChangeWords)+1}=funstrwords{i}{j};
      end
     end
     if tempflag~=0
      [funstr,funstrwords,funstrwords_b,funstrwords_e,funstrnumbers,funstrnumbers_b,funstrnumbers_e,s,fs_good]=updatefunstr_o(funstr,funstrwords,funstrwords_b,funstrwords_e,funstrnumbers,funstrnumbers_b,funstrnumbers_e,fs_good,i);
     end
    end % if ~incomment_o(funstr{i},
   end % if ~inastring_o(funstr{i},
  end % if ~inaDQstring_o(funstr{i},
 end % for j=length(funstrwords{i}):-1:1
end
if temp3
 [funstr,funstrwords,funstrwords_b,funstrwords_e,funstrnumbers,funstrnumbers_b,funstrnumbers_e,s,fs_good]=updatefunstr_o(funstr);
end

% if ' is in a double quote string " ", then change to ''
for i=fliplr(fs_good)
 if ~isempty(funstr{i})
  temp1=strfind(funstr{i},'''');
  if ~isempty(temp1)
   for jj=length(temp1):-1:1
    if inaDQstring_o(funstr{i},temp1(jj))
     funstr{i}=[funstr{i}(1:temp1(jj)-1),'''''',funstr{i}(temp1(jj):end)];
    end % if inaDBDQstring_o(funstr{ii},
   end % for jj=length(temp1):-1:1
  end % if ~isempty(temp1)
 end % if ~isempty(funstr{ii})
end % for i=fliplr(fs_good)

%fix the " to be '
funstr=strrep(funstr,'"','''');





%Construct filestr
filestr=[];
for i=1:s
  filestr=[filestr,funstr{i},r];
end

%save the original before overwriting.
%disp(['    copying original ',filename,' ==> ',r,'                     ',filename,'.PREo2m'])
%unix(['cp ',filename,' ',filename,'.PREo2m']);
copyfile(filename,[filename,'.PREo2m']);


%Write converted file out 
% some small housekeeping tasks
fprintf(1,'    Writing file:  ');   fprintf(1,filename);   fprintf(1,' ... ')
fid=fopen(filename,'w');  fprintf(fid,'%c',filestr);   fclose(fid);
fprintf(1,'completed \n')
disp(['   Total time: ',num2str(cputime-tt1)])
%showall_o(funstr),keyboard

%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%End oct2ml.


%'sssssssssss',funstr{i},keyboard








function out=findlefts_o(locs,str)
closestr=str(locs(1));
switch closestr
 case ')'
  openstr='(';
 case ']'
  openstr='[';
 case '}'
  openstr='{';
end
l=(str==openstr);    r=(str==closestr);
both=l-r;            c=cumsum(both);
fc=[fliplr(c) 0];
for i=1:length(locs)
 floc=length(str)-locs(i)+1;
 found=find(fc==(fc(floc)));
 found=found(found>floc+1);
 out(i)=length(str)-found(1)+1+1;
end



function out=findrights_o(locs,str,ignoreInQuotes)

if nargin<3, ignoreInQuotes=0; end
openstr=str(locs(1));

switch openstr
 case '('
  closestr=')';
 case '['
  closestr=']';
 case '{'
  closestr='}';
end
if ignoreInQuotes
 temp5=str=='''';
 temp6=cumsum(temp5);
 temp7=temp6/2~=round(temp6/2);

 l=(str==openstr) & ~temp7;
 r=(str==closestr) & ~temp7;
else
 l=(str==openstr);
 r=(str==closestr);
end
both=l-r;            c=cumsum(both);
for i=1:length(locs)
 found=find(c==(c(locs(i))-1));
 found=found(found>locs(i));
 if ~isempty(found)
  out(i)=found(1);
 else
  out(i)=0;
 end
end




function [howmany,subscripts,centercomma,parens]=hassubscript_o(i,whichword,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e)
howmany=0;subscripts=[];centercomma=[];parens=[];out=0;

temp=strfind(funstr{i},'(');

if ~isempty(temp)
 temp5=funstr{i}=='''';
 temp6=cumsum(temp5);
 temp7=temp6/2~=round(temp6/2);

 temp=temp(temp>funstrwords_e{i}(whichword)&~temp7(temp));
 if ~isempty(temp)
  temp=temp(1);
  temp1=funstr{i}(funstrwords_e{i}(whichword)+1:temp-1);
  temp2=length(temp1);
  if temp2==0 
   out=1;
  elseif all(isspace(temp1))
   out=1;
  end

  if out
   parens(1)=temp;
   rightparen=findrights_o(temp,funstr{i},1);
   parens(2)=rightparen;
   left=funstr{i}=='(' & ~temp7;
   right=funstr{i}==')' & ~temp7;
   both_p=left-right;                 c_p=cumsum(both_p);
   leftbracket=funstr{i}=='[' & ~temp7;
   rightbracket=funstr{i}==']' & ~temp7;
   both_b=leftbracket-rightbracket;   c_b=cumsum(both_b);
   commas=find(funstr{i}==',');
   commas=commas(((commas>parens(1))&(commas<parens(2))));
   commas=commas(find(~temp7(commas)));
   if ~isempty(commas)
    centercomma=commas(((c_p(commas)==c_p(parens(1)))&(c_b(commas)==c_b(parens(1)))));
   else
    centercommas=[];
   end
   howmany=length(centercomma)+1;
   for j=1:howmany
    if howmany==1
     subscripts{j}=funstr{i}(parens(1)+1:parens(2)-1);
    else
     if j==1
      subscripts{j}=funstr{i}(parens(1)+1:centercomma(1)-1);
     elseif j==howmany
      subscripts{j}=funstr{i}(centercomma(j-1)+1:parens(2)-1);
     else
      subscripts{j}=funstr{i}(centercomma(j-1)+1:centercomma(j)-1);
     end
    end
   end
   if howmany==1
    if length(find(~isspace(subscripts{1})))==0
     howmany=0;
    end % if length(find(~isspace(subscripts{1})))==0
   end % if howmany==1
  end
 end
end




function out=inaDQstring_o(str,loc)

out=[];
temp=str=='"';
temp1=cumsum(temp);
for ii=1:length(loc)
 if temp1(loc(ii))/2 ~= round(temp1(loc(ii))/2)
  out(ii)=1;
 else
  out(ii)=0;
 end
end






function out=inastring_o(str,loc)

out=[];
temp=str=='''';
temp1=cumsum(temp);
for ii=1:length(loc)
 if temp1(loc(ii))/2 ~= round(temp1(loc(ii))/2)
  out(ii)=1;
 else
  out(ii)=0;
 end
end





function [outflag,howmany,subscripts,centercomma,parens]=inbracket_o(i,spot,funstr);

outflag=0;
temp=findstr(funstr{i},'[');
temp1=findstr(funstr{i},']');
if length(temp(temp<spot))>length(temp1(temp1<spot))
 outflag=1;
end
howmany=0;subscripts=[];centercomma=[];parens=zeros(1,2);
if outflag
 if funstr{i}(spot)=='[', temp3=1; else temp3=0; end
 found=0;
 leftbracket=funstr{i}=='[';
 rightbracket=funstr{i}==']';
 both_b=leftbracket-rightbracket;   c_b=cumsum(both_b);
 poss=leftbracket&(c_b==(c_b(spot)-temp3));
 poss_loc=find(poss);
 poss_loc=poss_loc(poss_loc<spot);
 try
 parens(1)=poss_loc(end);
 catch
  poss_loc,kb
 end
 parens(2)=findrights(parens(1),funstr{i});
 tempstr=funstr{i};
 tempstr(1:parens(1))='0';
 tempstr(parens(2):end)='0';
 leftp=tempstr=='(';
 rightp=tempstr==')';
 both_p=leftp-rightp;                 c_p=cumsum(both_p);
 temp=length(findstr(':',funstr{i}(parens(1):parens(2))));
 if temp==0
  howmany=1;
  subscripts{1}=funstr{i}(parens(1)+1:parens(2)-1);
 elseif temp>0
  centercomma=findstr(':',funstr{i});
  centercomma=centercomma(((centercomma<parens(2))&(centercomma>parens(1))));
  found=1;  cc2=[];
  for k=1:length(centercomma)
   %Make sure we are not in any parenthesis group or any other bracket group
   if ((c_b(centercomma(k))==c_b(parens(1)))&(c_p(centercomma(k))==0))
    cc2(found)=centercomma(k);
    found=found+1;
   end
  end
  centercomma=cc2;
  howmany=length(centercomma)+1;
  if howmany==1
   subscripts{1}=funstr{i}(parens(1)+1:parens(2)-1);
  elseif howmany==2
   subscripts{1}=funstr{i}(parens(1)+1:centercomma(1)-1);
   subscripts{2}=funstr{i}(centercomma(1)+1:parens(2)-1);
  elseif howmany==3
   subscripts{1}=funstr{i}(parens(1)+1:centercomma(1)-1);
   subscripts{2}=funstr{i}(centercomma(1)+1:centercomma(2)-1);
   subscripts{3}=funstr{i}(centercomma(2)+1:parens(2)-1);
  end
 end
end





function out=incomment_o(str,locs)

out=zeros(1,length(locs));
temp=find(str=='%');
temp=temp(~inastring_o(str,temp));
if ~isempty(temp)
 for ii=1:length(locs)
  if min(temp)<locs(ii)
   out(ii)=1;
  else
   out(ii)=0;
  end
 end
end






function outflag=insubscript_o(i,spot,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e,funwords,filename,left,right);
needparentest=0;
if nargin<3
 needparentest=1;
end
outflag=0;
needtest=0;
leftpp=funstr{i}(1:spot)=='(';
rightpp=funstr{i}(1:spot)==')';
if needparentest
 leftp=length(find(leftpp));
 rightp=length(find(rightpp));
 if leftp>rightp
  needtest=1;
 end
else
  needtest=1;
end
if needtest
 if nargin<12
  temp=find(funstrwords_b{i}<spot);
 else
  temp=find(((funstrwords_b{i}<right)&(funstrwords_b{i}>left)));
 end
 if ~isempty(temp)  
  both_p=leftpp-rightpp;                 c_p=cumsum(both_p);
  last0=max(find(c_p==0));
  leftp_loc=find(leftpp);
  leftp_loc=leftp_loc(leftp_loc>last0);%Now only those which start after the last 0
  leftp_loc=leftp_loc(c_p(leftp_loc)<=c_p(spot));%Now only those which are left of closed groups
  %Now lets run through them. Last check is to see if each closes before spot
  for j=length(leftp_loc):-1:1
   if ~outflag
    if length(find(c_p(leftp_loc(j)+1:spot-1)<c_p(leftp_loc(j))))==0
     %Ready to go with this open paren
     temp=find(funstrwords_b{i}<leftp_loc(j));
     if ~isempty(temp)
      if (length(find(strcmp(funstrwords{i}(temp(end)),inoutother3)))>0)
       [howmany,subscripts,centercomma,parens]=hassubscript_o(i,temp(end),funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e);
       if howmany>0
	if ((parens(1)<spot)&(parens(2)>spot))
	 outflag=1;
	end
       end
      end
     end
    end
   end
  end
 end
end







function [outflag,howmany,subscripts,centercomma,parens]=inwhichlast_o(i,spot,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e,filename_all);

testcon=1;
outflag=0;
howmany=0;subscripts=[];centercomma=[];parens=[];

temp5=funstr{i}=='''';
temp6=cumsum(temp5);
temp7=temp6/2~=round(temp6/2);

left=funstr{i}=='(' & ~temp7;
right=funstr{i}==')' & ~temp7;
both_p=left-right;                 c_p=cumsum(both_p);
leftbracket=funstr{i}=='[' & ~temp7;
rightbracket=funstr{i}==']' & ~temp7;
both_b=leftbracket-rightbracket;   c_b=cumsum(both_b);

goon=0;
if (length(find(find(leftbracket)<spot))>length(find(find(rightbracket)<spot)))|(length(find(find(left)<spot))>length(find(find(right)<spot)))
 goon=1;
 % If there is a ( or [ on spot, we have to subtract 1
 if length(find(spot==find(left)))>0,  temp=1; else temp=0; end
 poss_p=find(left&(c_p==(c_p(spot)-temp)));
 poss_p=poss_p(poss_p<spot);
 poss_p=[0 poss_p];
 poss_p=poss_p(end);
 temp=0;
 if ~isempty(find(leftbracket))
  if length(find(spot==find(leftbracket)))>0,  temp=1;  end
 end
 poss_b=find(leftbracket&(c_b==(c_b(spot)-temp)));
 poss_b=poss_b(poss_b<spot);
 poss_b=[0 poss_b];
 poss_b=poss_b(end);
 j=max([poss_p poss_b]);
 if j==poss_b
  outflag=2; %bracket last
 else
  outflag=1; %paren last
 end
 parens(1)=j;
 if j~=0
  parens(2)=findrights_o(j,funstr{i});
 else
  parens(2)=0; outflag=0;
 end
end








function out=iskeep_o(str)
if ~isempty(str)
 out=((isletter(str))|(str=='_')|(str=='.')|((str>47)&(str<58)));
else
 out=[];
end





function funstr=replaceword_o(i,j,funstr,funstrwords,funstrwords_b,funstrwords_e,repstr)
funstr{i}=[funstr{i}(1:(funstrwords_b{i}(j)-1)),repstr,funstr{i}((funstrwords_e{i}(j)+1):length(funstr{i}))];





function showall(funstr,indented)
if nargin==0
 for i=1:size(funstr,1)
  disp(funstr{i})
 end
else
 for i=1:size(funstr,1)
  disp(['  ',funstr{i}])
 end
end






function w
whos





function [funstr,funstrwords,funstrwords_b,funstrwords_e,funstrnumbers,funstrnumbers_b,funstrnumbers_e,s,fs_good]=updatefunstr_o(funstr,funstrwords,funstrwords_b,funstrwords_e,funstrnumbers,funstrnumbers_b,funstrnumbers_e,fs_good,oneline)

if any(strfind(version,'R14'))
 s=length(funstr);
 to0out={'.*','./','.''','.^'};
 numstr='(\<(\d+\.\d+|\d+\.|\.\d+|\d+)([eEdDqQ][+-]?\d+)?)';
 wordstr='(\<[a-z_A-Z]\w*)';
 if nargin==1
  lo=1;hi=s;fs_good=[];
  funstrwords=cell(s,1);funstrwords_b=cell(s,1);funstrwords_e=cell(s,1);
  funstrnumbers=cell(s,1);funstrnumbers_b=cell(s,1);funstrnumbers_e=cell(s,1);
 else
  lo=oneline;hi=oneline;
  fs_good=fs_good(fs_good<lo|fs_good>hi);
  funstrwords{oneline}=cell(0);
  funstrwords_b{oneline}=[];funstrwords_e{oneline}=[];
  funstrnumbers{oneline}=cell(0);
  funstrnumbers_b{oneline}=[];funstrnumbers_e{oneline}=[];
 end

 empty=cellfun('isempty',{funstr{lo:hi}});
 bad1=strncmp({funstr{lo:hi}},'!',1);
 bad2=strncmp({funstr{lo:hi}},'%',1);
 bad3=strncmp(strtrim({funstr{lo:hi}}),'%',1);
 good=~(empty|bad1|bad2|bad3);
 ind=lo:hi;
 fs_good=[fs_good,ind(good)];
 goodind=ind(good);

 if length(fs_good)/s > 1/2 & nargin==1 %do all the lines
  [funstrnumbers,funstrnumbers_b,funstrnumbers_e]=regexp(funstr,numstr,'match','start','end');
  [funstrwords,funstrwords_b,funstrwords_e]=regexp(funstr,wordstr,'match','start','end');
 else %do only those lines defined by goodind 
  if (hi-lo) > 0
   [funnum1,funnum2,funnum3]=regexp({funstr{goodind}},numstr,'match','start','end');
   [funstrnumbers{goodind}]=deal(funnum1{:});
   [funstrnumbers_b{goodind}]=deal(funnum2{:});
   [funstrnumbers_e{goodind}]=deal(funnum3{:});

   [funword1,funword2,funword3]=regexp({funstr{goodind}},wordstr,'match','start','end');
   [funstrwords{goodind}]=deal(funword1{:});
   [funstrwords_b{goodind}]=deal(funword2{:});
   [funstrwords_e{goodind}]=deal(funword3{:});
  elseif ~isempty(goodind)
   [funstrnumbers{goodind},funstrnumbers_b{goodind},funstrnumbers_e{goodind}]=regexp(funstr{goodind},numstr,'match','start','end');
   [funstrwords{goodind},funstrwords_b{goodind},funstrwords_e{goodind}]=regexp(funstr{goodind},wordstr,'match','start','end');
  end
 end
 fs_good=sort(fs_good);
 
else

 s=length(funstr);
 to0out={'.*','./','.''','.^'};
 if nargin==1
  lo=1;hi=s;fs_good=[];
  funstrwords=cell(s,1);funstrwords_b=cell(s,1);funstrwords_e=cell(s,1);
  funstrnumbers=cell(s,1);funstrnumbers_b=cell(s,1);funstrnumbers_e=cell(s,1);
 else
  lo=oneline;hi=oneline;
  fs_good=fs_good(fs_good<lo|fs_good>hi);
  funstrwords{oneline}=cell(0);
  funstrwords_b{oneline}=[];funstrwords_e{oneline}=[];
  funstrnumbers{oneline}=cell(0);
  funstrnumbers_b{oneline}=[];funstrnumbers_e{oneline}=[];
 end
 for i=lo:hi
  tempw_b=zeros(1,10);   tempw_e=zeros(1,10);
  tempn_b=zeros(1,10);   tempn_e=zeros(1,10);
  both=~isspace(funstr{i});
  if ~isempty(both)
   for j=1:length(to0out)
    temp=findstr(funstr{i},to0out{j});
    both(temp)=0;         both(temp+1)=0;
   end
   both=double(both&iskeep_o(funstr{i}));
   both(2:end)=both(2:end)-both(1:end-1);
   bothind=find(both==1);         ll=length(bothind);
   bothind2=find(both==-1);       ll2=length(bothind2);
   if ll2<ll, bothind2=[bothind2 length(both)+1]; end
   cw=1;                          cn=1;
   couple=0;
   for j=1:ll
    if isletter(funstr{i}(bothind(j)))
     funstrwords{i}{cw}=funstr{i}(bothind(j):bothind2(j)-1);
     tempw_b(cw)=bothind(j);
     tempw_e(cw)=bothind2(j)-1;
     cw=cw+1;
    else
     if couple==0
      if j~=ll
       if ~isletter(funstr{i}(bothind(j+1)))
        if (strcmpi(funstr{i}(bothind(j+1)-1),'e')|strcmpi(funstr{i}(bothind(j+1)-2),'e')|strcmpi(funstr{i}(bothind(j+1)-1),'d')|strcmpi(funstr{i}(bothind(j+1)-2),'d'))
         couple=1;
        end
       end
      end
      negsign=0;
      if bothind(j)>1
       if funstr{i}(bothind(j)-1)=='-'
        if bothind(j)>2
         temps=find(~isspace(funstr{i}));
         temps=temps(temps<bothind(j)-1);
         if ~isempty(temps)
          if ~(iskeep_o(funstr{i}(temps(end)))|(funstr{i}(temps(end))==')'))
           negsign=1;
          end
         else
          negsign=1;
         end         
        else
         negsign=1;
        end
       end
      end
      funstrnumbers{i}{cn}=funstr{i}(bothind(j)-negsign:bothind2(j+couple)-1);
      tempn_b(cn)=bothind(j)-negsign;
      tempn_e(cn)=bothind2(j+couple)-1;
      cn=cn+1;
      %'int1',funstr{i},j,negsign,funstrnumbers{i},kb
     else
      couple=0;
     end
    end
   end
   funstrwords_b{i}=tempw_b(1:cw-1);
   funstrwords_e{i}=tempw_e(1:cw-1);
   funstrnumbers_b{i}=tempn_b(1:cn-1);
   funstrnumbers_e{i}=tempn_e(1:cn-1);
   if any(~isspace(funstr{i}))
    temp10=funstr{i}(~isspace(funstr{i}));
    if ~isempty(temp10)
     if (temp10(1)~='!')&(temp10(1)~='%')
      fs_good=[fs_good,i];
     end
    end
   end
  end
 end
 fs_good=sort(fs_good);


end







function [funstr,outflag,needAllUpdated]=wordconverter_o(i,j,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e,formats);
% This will change the Matlab function to its Fortran equivalent syntax.
% outflag(1) is 1 if its taken care of here. 0 if not.
outflag=[0];r=char(10);s=length(funstr);
fidStr='fid_'; needAllUpdated=0;
global sp sp2
sp=''; sp2=' ';

switch funstrwords{i}{j}
 case {'endif','endfor','endfunction','endwhile'}
  repstr='end';outflag(1)=1;
  funstr=replaceword_o(i,j,funstr,funstrwords,funstrwords_b,funstrwords_e,repstr);  
 case {'is_vector'}
  repstr='isvector';outflag(1)=1;
  funstr=replaceword_o(i,j,funstr,funstrwords,funstrwords_b,funstrwords_e,repstr);  
 case {'is_scalar'}
  repstr='isscalar';outflag(1)=1;
  funstr=replaceword_o(i,j,funstr,funstrwords,funstrwords_b,funstrwords_e,repstr);  
 case {'rows'}
  outflag(1)=1;
  [howmany,subscripts,centercomma,parens]=hassubscript_o(i,j,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e);
  if howmany>0
   repstr='size';
   [howmany,subscripts,centercomma,parens]=hassubscript_o(i,j,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e);
   funstr{i}=[funstr{i}(1:(funstrwords_b{i}(j)-1)),repstr,funstr{i}(parens(1):parens(2)-1),',1',funstr{i}(parens(2):end)];
  end
 case {'columns'}
  outflag(1)=1;
  [howmany,subscripts,centercomma,parens]=hassubscript_o(i,j,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e);
  if howmany>0
   repstr='size';
   [howmany,subscripts,centercomma,parens]=hassubscript_o(i,j,funstr,funstrnumbers,funstrnumbers_b,funstrnumbers_e,funstrwords,funstrwords_b,funstrwords_e);
   funstr{i}=[funstr{i}(1:(funstrwords_b{i}(j)-1)),repstr,funstr{i}(parens(1):parens(2)-1),',2',funstr{i}(parens(2):end)];
  end
 case {'printf'}
  repstr='fprintf';outflag(1)=1;
  funstr=replaceword_o(i,j,funstr,funstrwords,funstrwords_b,funstrwords_e,repstr);  
end



