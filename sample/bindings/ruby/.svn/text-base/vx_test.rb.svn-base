#!/usr/bin/ruby -w

require 'openvx'
#include OpenVX;

def openvx_simple_demo
    graph = OpenVX::Graph.new
    input = OpenVX::Image.new(640, 480, OpenVX::Image::FOURCC_Y800)
    output = OpenVX::Image.new(640, 480, OpenVX::Image::FOURCC_Y800)
    node = OpenVX::CopyImageNode(graph, input, output)
    graph.verify
    graph.process
end

def openvx_debugging
    puts "OpenVX is a #{OpenVX.class}"
    OpenVX.constants.each {|c|
        puts "\tclass #{c}"
    }
    OpenVX::load("xyz")
    puts "There are #{OpenVX.num_kernels} kernels in OpenVX"
    puts "There are #{OpenVX.num_targets} targets in OpenVX"
    puts "There are #{OpenVX.num_modules} targets in OpenVX"
    kernel = OpenVX::Kernel.new("org.khronos.copy_image")
    puts "Copy image has #{kernel.params} parameters"
    graph = OpenVX::Graph.new
    puts "There are #{graph.nodes} nodes in the graph"
    images = [] # new array
    puts "FOURCC_Y800 = #{OpenVX::Image::FOURCC_Y800}"
    # Image from named hash
    images << OpenVX::Image.new({:width => 640,
                                 :height => 480,
                                 :format => OpenVX::Image::FOURCC_Y800})
    # explicit typed parameters
    images << OpenVX::Image.new(640, 480, OpenVX::Image::FOURCC_Y800)
    # "Virtual"
    images << OpenVX::Image.new
    # node construction takes an array
    node = OpenVX::CopyImageNode(graph, images[0], images[1])
    graph.verify
    graph.process
    node = nil
    images.map! {|i| i=nil }
    graph = nil
end

if __FILE__ == $0
    if (ARGV[0] == '-s')
        openvx_simple_demo()
    else
        openvx_debugging()
    end
end
