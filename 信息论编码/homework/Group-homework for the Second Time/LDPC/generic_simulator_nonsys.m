clear all
close all
clc
tic
H = genH(50, 100);
ind=find(H==1);
[r,c]=ind2sub(size(H),ind);
[rows,cols]=size(H);
h=sparse(H);                        
n=cols;
k=n-rows;
dB=0:0.5:8;                           % SNR的范围（dB）
SNRpbit=10.^(dB/10);                
No_uncoded=1./SNRpbit;              % Eb=1
R=k/n;                              % 码率 
No=No_uncoded./R;
iter=10;                   %迭代次数
maximum_blockerror=100;          % 对于每个SNR下达到的最大的帧错误数
biterrors1=0;
blockerrors1=0;
biterrors2=0;
blockerrors2=0;
biterrors3=0;
blockerrors3=0;
biterrors4=0;
blockerrors4=0;
biterrors5=0;
blockerrors5=0;
biterrors6=0;
blockerrors6=0;
block1=0;
block2=0;
block3=0;
block4=0;
block5=0;
block6=0;
FER1=zeros(1,length(dB));       
BER1=zeros(1,length(dB)); 
FER2=zeros(1,length(dB));       
BER2=zeros(1,length(dB)); 
FER3=zeros(1,length(dB));       
BER3=zeros(1,length(dB)); 
FER4=zeros(1,length(dB));       
BER4=zeros(1,length(dB));
FER5=zeros(1,length(dB));       
BER5=zeros(1,length(dB));
FER6=zeros(1,length(dB));       
BER6=zeros(1,length(dB));
block_array1=zeros(1,length(dB));
block_array2=zeros(1,length(dB));
block_array3=zeros(1,length(dB));
block_array4=zeros(1,length(dB));
block_array5=zeros(1,length(dB));
block_array6=zeros(1,length(dB));
%%%%%%%%%%%%%%%%%%采用比特翻转译码算法%%%%%%%%%%%%%%%%%%%%%%%
 
%%%%%%%%%%%%%%%%%%采用概率域BP译码算法%%%%%%%%%%%%%%%%%%%%%%%
for z=1:length(SNRpbit) % 对SNR循环
rand('seed',21);
randn('seed',80);      
    biterrors1=0;
    blockerrors1=0;
    block1=0;
    biterrors2=0;
    blockerrors2=0;
    block2=0; 
        biterrors3=0;
    blockerrors3=0;
    block3=0;
        biterrors4=0;
    blockerrors4=0;
    block4=0;
        biterrors5=0;
    blockerrors5=0;
    block5=0;
        biterrors6=0;
    blockerrors6=0;
    block6=0;
    while(block2<maximum_blockerror)  %帧错误数小于最大设定的帧错误数时进入循环
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% 对消息进行编码 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        s=round(rand(1, cols-rows));%随机产生一个消息序列
        %使用H矩阵进行LDPC编码
        [u,P,rearranged_cols]=ldpc_encode(s,H);  
        %对编码序列进行bpsk调制
        tx_waveform=bpsk(u);        
        %将调制好的序列发送如AWGN信道
        sigma=sqrt(No(z)/2);
        rx_waveform=tx_waveform + sigma*randn(1,length(tx_waveform));
        tx=rx_waveform';
        N0=No(z);
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%译码%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
         %比特翻转译码算法
         %vhat1 = decodeBitFlip(tx, H,iter);        
         %uhat1=vhat1;
         %BP译码算法
         vhat2 = decodeProbDomain(tx, H, N0, iter);
         uhat2=vhat2;
         %对数域BP译码算法
         %vhat3 = decodeLogDomain(tx, H, N0, iter);
         %uhat3=vhat3;
         %最小和译码算法
         vhat4 = decodeMinSumLogDomain(tx, H, N0, iter);
         uhat4=vhat4;
         %归一化最小和译码算法
         vhat5 =decodeNormalMinSumLogDomain(tx, H, N0, iter);
         uhat5=vhat5;
         %改进的译码算法
         %vhat6 =decodeModMinSumLogDomain(tx, H, N0, iter);
         %uhat6=vhat6;
 %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        %[num1, rat1] = biterr(vhat1, u);
        [num2, rat2] = biterr(vhat2, u);
%        [num3, rat3] = biterr(vhat3, u);        
        [num4, rat4] = biterr(vhat4, u);
        [num5, rat5] = biterr(vhat5, u);   
        %[num6, rat6] = biterr(vhat6, u);
     %   if num1~=0
      %      blockerrors1=blockerrors1+1;
       % end       
       % biterrors1=biterrors1+num1;
        %block1=block1+1;
        if num2~=0
            blockerrors2=blockerrors2+1;
        end   
        biterrors2=biterrors2+num2;
        block2=block2+1;
   %     if num3~=0
    %        blockerrors3=blockerrors3+1;
     %   end   
      %  biterrors3=biterrors3+num3;
       % block3=block3+1;
        if num4~=0
            blockerrors4=blockerrors4+1;
        end
        biterrors4=biterrors4+num4;
        block4=block4+1;
        if num5~=0
            blockerrors5=blockerrors5+1;
       end   
       biterrors5=biterrors5+num5;
       block5=block5+1;
     %   if num6~=0
   %        blockerrors6=blockerrors6+1;
    %   end   
    %    biterrors6=biterrors6+num6;
     %  block6=block6+1;
    end    
    block_array1(z)=block1; 
    BER1(z)=biterrors1/(block1*n);
    FER1(z)=blockerrors1/block1;
    block_array2(z)=block2; 
    BER2(z)=biterrors2/(block2*n);
    FER2(z)=blockerrors2/block2;
     block_array4(z)=block4;
     block_array3(z)=block3;
    BER3(z)=biterrors3/(block3*n);
    FER3(z)=blockerrors3/block3;   
    BER4(z)=biterrors4/(block4*n);
    FER4(z)=blockerrors4/block4;
        block_array5(z)=block5;
    BER5(z)=biterrors5/(block5*n);
    FER5(z)=blockerrors5/block5;
        block_array6(z)=block6;
    BER6(z)=biterrors6/(block6*n);
    FER6(z)=blockerrors6/block6;
end 
figure(1)
semilogy(dB,BER4,'r-o')
hold on
semilogy(dB,BER2,'b-o')
hold on
semilogy(dB,BER5,'g-o')
hold on
semilogy(dB,FER4,'r--*')
hold on
semilogy(dB,FER2,'b--*')
hold on
semilogy(dB,FER5,'g--*')
hold on
title('误比特率/误分组率')
ylabel('误比特率/误分组率')
xlabel('信噪比 (dB)')
grid
toc


