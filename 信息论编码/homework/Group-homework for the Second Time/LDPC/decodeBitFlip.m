function vHat = decodeBitFlipping(rx, H, iteration)
% Hard-decision/bit flipping sum product algorithm LDPC decoder
%
%  rx        : Received signal vector (column vector)
%  H         : LDPC matrix
%  iteration : Number of iteration
%
%  vHat      : Decoded vector (0/1) 
%
%
% Copyright Bagawan S. Nugroho, 2007 
% http://bsnugroho.googlepages.com

[M N] = size(H);

% Prior hard-decision
ci = 0.5*(sign(rx') + 1);

% Initialization
rji = zeros(M, N);

% Asscociate the ci matrix with non-zero elements of H
qij = H.*repmat(ci, M, 1);
 
% Iteration
for n = 1:iteration
   
   fprintf('Iteration : %d\n', n);
   
   % ----- Horizontal step -----
   for i = 1:M
      
      % Find non-zeros in the column
      c1 = find(H(i, :));
      
      % Get the summation of qij\c1(k)        
      for k = 1:length(c1)

         rji(i, c1(k)) = mod(sum(qij(i, c1)) + qij(i, c1(k)), 2);
         
      end % for k
      
   end % for i
   
   % ------ Vertical step ------
   for j = 1:N

      % Find non-zero in the row
      r1 = find(H(:, j));
      
      % Number of 1s in a row
      numOfOnes = length(find(rji(r1, j)));
      
      for k = 1:length(r1)        

         % Update qij, set '1' for majority of 1s else '0', excluding r1(k)
         if numOfOnes + ci(j) >= length(r1) - numOfOnes + rji(r1(k), j)
            qij(r1(k), j) = 1;
         else
            qij(r1(k), j) = 0;
         end
          
      end % for k
      
      % Bit decoding
      if numOfOnes + ci(j) >= length(r1) - numOfOnes
         vHat(j) = 1;
      else
         vHat(j) = 0;
      end
             
   end % for j
   
end % for n
