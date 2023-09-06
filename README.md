# TwoPhaseLocking


## 1. 任务分析

1. 事务中既包含读操作，也包含写操作
2. 读写操作可能落在同一个索引位置
3. 读操作比写操作多，且每次的读的位置在绝大部分情况下是连续的，除非i = 99999


## 2. 锁机制设计

参考两端锁协议的“扩张阶段只加锁不放锁，收缩阶段只放锁不加锁”，使用最多四个锁（三个读锁，一个写锁），锁住该事务涉及到的数组位置。

加锁顺序为：RLock(i), RLock(i+1), Rlock(i+2), WLock(j)

解锁顺序为：WUnlock(j), RUnlock(i+2), RUnlock(i+1), RUnlock(i)

## 3. 死锁分析

死锁可能发生在读写操作落在同一个索引位置的情况。比如：


| transaction1      | transaction2 |
| ----------- | ----------- |
| Rlock(1)    | Rlock(2)       |
| Wlock(2)   | Wlock(1)        |

当事务1和事务2同时加上读锁之后，因为下一步需要加写锁，所以互相等待对方释放读锁，进而产生死锁。

死锁问题，可能的解决方法：

1. 全局锁，缺点是锁粒度太大，并发度太差
2. 后台监控线程，定期检测是否出现资源请求环图（dfs），但是在这个场景下环可能会非常长，算法的成本可能比较高
3. 超时回滚，如果加锁操作等待了一段时间后仍未得到锁，则释放持有的所有锁，等待随机时间后，再重新开始。主要缺点是等待时间的设定不好把控，或降低了系统的整体运行速度

最终选择超时回滚法解决本场景中的读写死锁问题。当i, i+1, i+2中有一个位置与j相等时，跳过加这个读锁，只加对应的写锁。


对C++14的超时读写锁shared_timed_mutex进行了简单的封装，方便使用。

## 4. 分段锁

该场景下，同时访问到同一个数组位置的概率较低，因此可以使用分段锁/细粒度锁。

设计粒度锁时应关注的trade-off：加解锁操作的开销vs系统整体并发度

---

定义该场景中的粒度表示一个锁锁定连续数组元素的数量。

读操作一定访问三个连续位置，因此粒度最大应为3。即最坏的情况下，一次事务会加三个锁，而最好情况下，一次事务会加一个锁，即i,i+1,i+2,j落在同一个连续区间（长度3）。而粒度大于3是没有意义的，因为粒度更大的时候，一次事务最多也还是加三个锁，加锁操作的成本不会减少，但是由于粒度变大，单个锁的范围变大，会降低系统的并发度。


## 5. 测试

测试环境：docker ubuntu18

|      |  |
| ----------- | ----------- |
| 线程数  | 1024    |
| 数组大小   | 100000      |
|单线程触发事务次数|10000

测试结果：
| 粒度     | 运行时间（ms） |
| ----------- | ----------- |
|  1 | 267067    |
| 2   | 374648      |
| 3   |  409338     |

测试结果可能受笔记本运行环境影响，没有控制其他变量，推测测试结果的原因为锁粒度增大导致并行度降低，该负面影响高于增加的加锁操作的开销。更细致的分析需要结合系统分析工具如火焰图或者调试观察线程运行情况进一步判断。


