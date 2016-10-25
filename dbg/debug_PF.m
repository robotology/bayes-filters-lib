particles = importdata('result_particle.txt');
w = importdata('result_weight.txt');
object = importdata('result_object.txt');
meas = importdata('result_measurement.txt');

hold on;
plot(object(1, :), object(3, :));
plot(meas(1, :), meas(2, :), 'xr');
plot(particles(1, :), particles(3, :), 'xb');

hold on; box on;
xlim([min(object(1, :)) max(object(1, :))]);
ylim([min(object(3, :)) max(object(3, :))]);
plot(object(1, :), object(3, :));
plot(meas(1, :), meas(2, :), 'xr');
ext = [];
for k = 1:100
    h = plot3(particles((k-1)*4+1, :), particles((k-1)*4+3, :), w((k-1)*10000+1:(k-1)*10000+10000), 'xb');
    ext = [ext sum(repmat(w((k-1)*10000+1:(k-1)*10000+10000)', [4 1]) .* particles([(k-1)*4+1:(k-1)*4+4], :), 2)]; %#ok<AGROW>
    pause(0.1);
    delete(h);
end
comet(ext(1, :), ext(3, :));