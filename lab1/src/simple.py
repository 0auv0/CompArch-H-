import m5

from m5.objects import *

system = System()

#设置系统时钟域，设置系统时间
system.clk_domain = SrcClockDomain()
system.clk_domain.clock = "1GHz"
system.clk_domain.voltage_domain = VoltageDomain()


system.mem_mode = "timing"  #使用时序访问
system.mem_ranges = [AddrRange("512MB")]  #创建一个地址范围

#创建一个用于X86 ISA的CPU
system.cpu = X86TimingSimpleCPU()

system.membus = SystemXBar()

system.cpu.icache_port = system.membus.cpu_side_ports
system.cpu.dcache_port = system.membus.cpu_side_ports

system.cpu.createInterruptController()

# X86平台特有的中断控制器
system.cpu.interrupts[0].pio = system.membus.mem_side_ports
system.cpu.interrupts[0].int_requestor = system.membus.cpu_side_ports
system.cpu.interrupts[0].int_responder = system.membus.mem_side_ports

system.mem_ctrl = MemCtrl()
system.mem_ctrl.dram = DDR3_1600_8x8()
system.mem_ctrl.dram.range = system.mem_ranges[0]
system.mem_ctrl.port = system.membus.mem_side_ports

system.system_port = system.membus.cpu_side_ports

#创建进程
thispath = os.path.dirname(os.path.realpath(__file__))
binary = os.path.join(
    thispath,
    "../../../",
    "tests/test-progs/hello/bin/x86/linux/hello",
)

system.workload = SEWorkload.init_compatible(binary)

process = Process()
process.cmd = [binary]

system.cpu.workload = process
system.cpu.createThreads()

# 实例化一个全局的root对象
root = Root(full_system=False, system=system)
#开始实际模拟
m5.instantiate()

print(f"Beginning simulation!")
exit_event = m5.simulate()
print(f"Exiting @ tick {m5.curTick()} because {exit_event.getCause()}")