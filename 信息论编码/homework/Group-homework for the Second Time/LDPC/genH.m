function [H]=genH(rows,cols)

row_flag(1:rows)=0;
parity_check=zeros(rows,cols);

%add bits_per_col 1's to each column with the only constraint being that the 1's should be
%placed in distinct rows
%%%%%%%%ʹÿ���������3��1������Ϊ3%%%%%%%%%%%%%
bits_per_col=3;
for i=1:cols
   a=randperm(rows);
   for j=1:bits_per_col
      parity_check(a(j),i)=1;
      row_flag(a(j))=row_flag(a(j))+1;
   end
end

%����ÿ��1��������
max_ones_per_row=ceil(cols*bits_per_col/rows);

%add 1's to rows having no 1(a redundant row) or only one 1(that bit in the codeword becomes
%zero irrespective of the input)
for i=1:rows
   if row_flag(i)==0    %�������û��1��������������1
     for k=1:2
        j=unidrnd(cols);%��ָ����������Χ�ڲ���һ�������
        while parity_check(i,j)==1
            j=unidrnd(cols);
        end
        parity_check(i,j)=1;        %���ҵ�����λ������1
        row_flag(i)=row_flag(i)+1;  %���ؼ�1
      end
   end
   if row_flag(i)==1    %�������ֻ��1��1������������1��1
      j=unidrnd(cols);
      while parity_check(i,j)==1
         j=unidrnd(cols);
      end
      parity_check(i,j)=1;
      row_flag(i)=row_flag(i)+1;
   end
end

%try to distribute the ones so that the number of ones per row is as uniform as possible
%���������Ϸ�ɢ1��λ�ã�ʹ��ÿ��1�ĸ������⣨�������һ�£�
for i=1:rows
   j=1;
   a=randperm(cols);
   while row_flag(i)>max_ones_per_row;  %����������ش��������������أ�����д���
      if parity_check(i,a(j))==1 %���ѡ��ĳһ������Ϊ1���������������и����ϵ�1��ɢ����������
         %������Ҹ������ʺϷ���1������С�������������أ��Ҹ�λ����Ϊ0������
         newrow=unidrnd(rows);
         k=0;
         while (row_flag(newrow)>=max_ones_per_row | parity_check(newrow,a(j))==1) & k<rows
            newrow=unidrnd(rows);
            k=k+1;
         end
         if parity_check(newrow,a(j))==0
            %�����������ϵ�1ת�ŵ��ҵ�������
            parity_check(newrow,a(j))=1;
            row_flag(newrow)=row_flag(newrow)+1;
            parity_check(i,a(j))=0;
            row_flag(i)=row_flag(i)-1;
         end
      end%if test
      j=j+1;
   end%while loop
end%for loop

%try to eliminate cycles of length 4 in the factor graph
%����ɾ��4��
for loop=1:10
   chkfinish=1;%�Ƿ�Ѱ���Ļ��ı�־λ
   for r=1:rows
      ones_position=find(parity_check(r,:)==1);%�ҳ�r���е�1��λ��
      ones_count=length(ones_position);%r����1������
      for i=[1:r-1 r+1:rows]%�൱��������
         common=0;
         for j=1:ones_count%�൱��������
            if parity_check(i,ones_position(j))==1%��ͬλ���ϴ���1
               common=common+1 ;
               if common==1
                  thecol=ones_position(j);%�Ѹ�λ�ø���thecol
               end
            end
            if common==2
               chkfinish=0; %���������4�����򲻽���ѭ������������һ��ѭ��
               common=common-1;
               if (round(rand)==0)           % ��������Ǳ���ǰ����л��Ǻ������
                  coltoberearranged=thecol;           %����������У�����ǰ�����
                  thecol=ones_position(j);
               else
                  coltoberearranged=ones_position(j); %����ǰ����У������������
               end
               parity_check(i,coltoberearranged)=3; %make this entry 3 so that we dont use
                                                    %of this entry again while getting rid
                                                    %of other cylces
               newrow=unidrnd(rows);%��ָ����������Χ�ڲ���һ�������
               iteration=0;     %����5���ڴ������������������0
               while parity_check(newrow,coltoberearranged)~=0 & iteration<5
                  newrow=unidrnd(rows);
                  iteration=iteration+1;
               end
               if iteration>=5  %����5�κ�������Χ������ҷ�1��0��3��ֱ���ҵ�Ϊֹ
                  while parity_check(newrow,coltoberearranged)==1
                     newrow=unidrnd(rows);
                  end
               end
               %�Ѹ������ҵ���0��3��Ϊ1
               parity_check(newrow,coltoberearranged)=1;
            end%if common==2
         end%for j=1:ones_count
      end%for i=[1:r-1 r+1:rows]
   end%for r=1:rows

   %�������ѭ���Ѳ�����4���������ѭ������������һ��ѭ��
   if chkfinish
      break
   end
end%for loop=1:10

%replace the 3's with 0's
parity_check=parity_check==1;

%Get the Parity Checks
H=parity_check;

%%%%%������󷽲����������%%%%
%%��������
%col_flag(1:cols)=0;
%for j=1:cols
%   ind=find(H(:,j)==1);
%   col_flag(j)=length(ind);
%end
%%��������
%row_flag(1:rows)=0;
%for i=1:rows
%   ind=find(H(i,:)==1);
%   row_flag(i)=length(ind);
%end
%%ÿ��1�ĸ����ķ���
%variance=var(row_flag);
