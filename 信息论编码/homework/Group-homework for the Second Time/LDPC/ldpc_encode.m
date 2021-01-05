function [u,P,rearranged_cols]=ldpc_encode(s,H)
dim=size(H);
rows=dim(1);
cols=dim(2);

[P,rearranged_cols]=H2P(H);%高斯消元函数

c=mul_GF2(P,s');

u1=[c' s];

u=reorder_bits(u1,rearranged_cols);
