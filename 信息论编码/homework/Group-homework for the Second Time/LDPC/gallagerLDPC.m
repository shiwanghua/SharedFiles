% program to generate a parity check matrix by the Gallager approach
% Copyright MOHAMMED ALKHANBASHI 2007 
 
function [H]=gallagerLDPC(rows,cols, check)

ANS=1;
%====== getting the spec. of the parity check matrix from the user ======%
%=========================================================================%
while (ANS==1)
l=0;
    while l<1
    %d = inputdlg('Please Specify N'); %n:the number of bits in the codeword
    %d = 'cols';
    %n=str2num(d{1});
    n  = rows;
    %d = inputdlg('Please Specify C'); %c:the number of parity check bits
    %d = rows;
    %c=str2num(d{1});
    c = cols;
    %d = inputdlg('Please Specify the number of 1s in each columns');
    %d = check;
    %wc=str2num(d{1});               %wc: the number of ones in each columns
    wc = check;
    l=1;
    end

    wr=(n.*wc)./c;                  %wr:the number of ones in each row
    h=zeros(c,n);                   %generate the empty matrix and start 
                                    %assigning the 1s 
    
%=========================================================================%
%===========================ALGORITHM start here==========================%


    onesvector=ones(1,wr);
    start=1;
    finish=wr;   

    for ii = 1:c./wc

       h(ii,start:finish)=1;        
       start=start+wr;              
       finish=(ii.*wr)+wr;          
    end
    
%assign the ones in the first set
%for example:

  %   1     1     1     1     0     0     0     0     0     0     0     0
  %   0     0     0     0     1     1     1     1     0     0     0     0
  %   0     0     0     0     0     0     0     0     1     1     1     1
  %   -------------------------------------------------------------------
  %   0     0     0     0     0     0     0     0     0     0     0     0
  %   0     0     0     0     0     0     0     0     0     0     0     0
  %   0     0     0     0     0     0     0     0     0     0     0     0
  %   --------------------------------------------------------------------
  %   0     0     0     0     0     0     0     0     0     0     0     0
  %   0     0     0     0     0     0     0     0     0     0     0     0
  %   0     0     0     0     0     0     0     0     0     0     0     0

 %========================================================================%
 %===== assign the 1s randomly from the first set in the other sets ======%
  
  for i=1:wc-1
    r=1;

       for ii=1:n
       col_index = (round(rand(1) * (n-1) ))+1;
          
       randomCol=h(1:c./wc,col_index);
            
       h((i.*(c./wc))+1   :  ((i.*(c./wc))+1  +  (c./wc)-1),ii )=randomCol;
               
       end
       r=r+1;
  end
   
%example: assigning the ones in the second set 

%     1     1     1     1     0     0     0     0     0     0     0     0
%     0     0     0     0     1     1     1     1     0     0     0     0
%     0     0     0     0     0     0     0     0     1     1     1     1
%     --------------------------------------------------------------------
%     0     0     0     0     0     0     1     1     0     1     0     1
%     1     0     0     1     1     0     0     0     1     0     1     0
%     0     1     1     0     0     1     0     0     0     0     0     0
%     --------------------------------------------------------------------
%     0     0     0     0     0     0     0     0     0     0     0     0
%     0     0     0     0     0     0     0     0     0     0     0     0
%     0     0     0     0     0     0     0     0     0     0     0     0
   
%=========================================================================
h % display the Gallager parity check matrix 
   
%example
%      1     1     1     1     0     0     0     0     0     0     0     0
%      0     0     0     0     1     1     1     1     0     0     0     0
%      0     0     0     0     0     0     0     0     1     1     1     1
%      0     0     0     0     0     0     1     1     0     1     0     1
%      1     0     0     1     1     0     0     0     1     0     1     0
%      0     1     1     0     0     1     0     0     0     0     0     0
%      0     0     1     0     0     0     0     0     0     0     0     0
%      0     1     0     0     0     0     0     1     1     1     1     0
%      1     0     0     1     1     1     1     0     0     0     0     1
   
   
   %ans= inputdlg('DO You Want continue 1:"yes"    2:"no"');

   %ANS=str2num(ans{1});

 
   %if (ANS==2)
    break;
    end
    H=h;
end
