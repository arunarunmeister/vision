#!/usr/bin/ruby -w

require 'openvxruby'
puts "Loaded OpenVX Ruby Extensions!"

module OpenVX
    def self.kernel_names
        khash = {}
        OpenVX.targets.each do |target|
            target.kernels.each do |kernel|
                khash[kernel.name] ||= 0
                khash[kernel.name] += 1
            end
            target = nil
        end
        return khash.keys.sort
    end

    def self.targets
        array = []
        for index in 0..OpenVX.num_targets-1 do
            array << OpenVX::Target.new(index)
        end
        return array
    end

    class Target
        def kernels
            array = []
            kernel_names.each do |name|
                array << OpenVX::Kernel.new(name)
            end
            return array
        end
    end

    def self.CopyImageNode(graph, input, output)
        return nil if graph.class != OpenVX::Graph
        return nil if input.class != OpenVX::Image
        return nil if output.class != OpenVX::Image

        kernel = OpenVX::Kernel.new("org.khronos.copy_image")
        node = OpenVX::Node.new(graph, kernel, [{:dir => OpenVX::Parameter::VX_INPUT, :ref => input},
                                                {:dir => OpenVX::Parameter::VX_OUTPUT, :ref => output}])
        kernel = nil
        return node
    end
end

