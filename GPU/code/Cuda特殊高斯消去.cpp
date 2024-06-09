// 初始化数据
init_A();
init_P();

// 在设备上分配内存并拷贝数据
g_Act = allocate_device_memory<int>(lieNum * (Num + 1));
g_Pas = allocate_device_memory<int>(lieNum * (Num + 1));
copy_data_to_device(Act, g_Act);
copy_data_to_device(Pas, g_Pas);

// 定义并行执行的线程块和线程数
threads_per_block = 256;
number_of_blocks = 32;

// 记录开始时间
start = get_current_time();

bool sign;
do {
    // 拷贝数据到设备
    copy_data_to_device(Act, g_Act);
    copy_data_to_device(Pas, g_Pas);

    // 在CUDA设备上并行执行工作项
    parallel_for(1D grid 和 block 的设置) {
        work(Num, pasNum, lieNum, g_Act, g_Pas, threadId);
    }

    // 同步设备，等待工作项执行完成
    device_synchronize();

    // 将结果从设备拷贝回主机
    copy_data_to_host(g_Act, Act);
    copy_data_to_host(g_Pas, Pas);

    // 判断是否需要继续迭代
    sign = false;
    for (int i = 0; i < pasNum; i++) {
        int temp = Pas[i * (Num + 1) + Num];
        if (temp == -1) {
            continue;
        }
        if (Act[temp * (Num + 1) + Num] == 0) {
            for (int k = 0; k < Num; k++) {
                Act[temp * (Num + 1) + k] = Pas[i * (Num + 1) + k];
            }
            Pas[i * (Num + 1) + Num] = -1;
            sign = true;
        }
    }

} while (sign == true);

// 记录结束时间
stop = get_current_time();

// 输出执行时间
execution_time = calculate_execution_time(start, stop);
print("GPU_LU: ", execution_time, " ms");