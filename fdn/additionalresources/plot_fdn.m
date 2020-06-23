% plot_fdn.m
% created by tolka nabroski
clear all; close all; clc;
fs = 48000;
% Create an impulse 
x = zeros(1, 1 * fs);
x(1) = .354 * 1;
y = zeros(1, fs);
% feedback matrix 
A = [1e-6 -0.685894 0.685894 1e-6; 0.685894 1e-6 1e-6 0.685894; 0.685894 1e-6 1e-6 -0.685894; 1e-6 -0.685894 -0.685894 1e-6];
% delay lines, use prime numbers 
m = [577 601 641 661]'; 
z1 = zeros(1, 661);
z2 = zeros(1, 661);
z3 = zeros(1, 661);
z4 = zeros(1, 661);
for n = 1 : length(y)
tmp = [z1(m(1)) z2(m(2)) z3(m(3)) z4(m(4))];
y(n) = .354 * (z1(m(1)) + z2(m(2)) + z3(m(3)) + z4(m(4)));
z1 = [(x(n) + tmp * A(1,:)') z1(1:length(z1)-1)];
z2 = [(x(n) + tmp * A(2,:)') z2(1:length(z2)-1)];
z3 = [(x(n) + tmp * A(3,:)') z3(1:length(z3)-1)];
z4 = [(x(n) + tmp * A(4,:)') z4(1:length(z4)-1)];
end
plot(y);
legend('impulse');
