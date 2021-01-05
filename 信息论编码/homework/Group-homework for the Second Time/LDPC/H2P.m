function [P,rearranged_cols]=H2P(H)

dim=size(H);
rows=dim(1);
cols=dim(2);

rearranged_cols=zeros(1, rows);%1��rows�������

%���н��и�˹��Ԫ��ǰ���rows�� x rows���γɵ�λ����
for k=1:rows
    vec = [k:cols];

    %���ҿɽ�������
    x = k;
    while (x<=cols & H(k,x)==0)
        ind = find(H(k+1:rows, x) ~= 0);
        if ~isempty(ind)%����Ϊ�գ�����ֵΪ��
            break
        end
        x = x + 1;
    end

    %����Ҳ����ɽ�����������Ϊ�Ƿ���H�����˳�
    if x>cols
        error('Invalid H matrix.');
    end

    %������ǵ�ǰ��������н�����ͬʱ���潻����¼
    if (x~=k)
        rearranged_cols(k)=x;
        temp=H(:,k);
        H(:,k)=H(:,x);
        H(:,x)=temp;
    end

    %��˹��Ԫ��ʹG(k,k)==1
    if (H(k,k)==0)
        ind = find(H(k+1:rows, k) ~= 0);
        ind_major = ind(1);
        x = k + ind_major;
        H(k, vec) = rem(H(x, vec) + H(k, vec), 2);%��������
    end

    %��˹��Ԫ��ʹ�õ�k�г�G(k,k)==1������λ��Ϊ0
    ind = find(H(:, k) ~= 0)';
    for x = ind
        if x ~= k
            H(x, vec) = rem(H(x, vec) + H(k, vec), 2);
        end
    end
end

P=H(:,rows+1:cols);
