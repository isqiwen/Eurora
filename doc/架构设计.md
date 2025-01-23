# 多维张量的实时处理通用模型


## 1. 数据表示

多维张量 $D$ 表示为：

$$
D \in \mathbb{C}^{d_1 \times d_2 \times \cdots \times d_n},
$$

其中：
- $n$ 是张量的维度数；
- $d_i$ 是张量在第 $i$ 个维度上的大小；
- 张量可以包含实数或复数值 $D \in \mathbb{R}$ 或 $D \in \mathbb{C}$。


数据维度集合$ S_{\text{dims}} = \{d_1, d_2, \dots, d_n\} $代表了数据属性的完整集合。

---

## 2. 数据实时性

数据实时性要求：
- **增量到达**：数据以块的形式逐步到达，每个块 $\Delta D$ 是张量的一个子集：
  $$
  \Delta D \subseteq D,
  $$
  其中 $\Delta D$ 的大小和维度由数据源决定。

---

## 3. 处理阶段

多维张量处理分为多个阶段，每个阶段对张量进行特定的操作。

### $ Operator $ 定义

$$
O = \{S_{\text{block}}, S_{\text{aggregated}}, S_{\text{pending}}, S_{\text{expand}}, S_{\text{parallel}}, f, \text{Index}\},
$$

其中：
- **$S_{\text{block}}$**：块维度集合，表示当前阶段被视为最小数据单元的维度。  $ S_{\text{block}} \subseteq D \setminus A $。
- **$S_{\text{aggregated}}$**：已聚合维度集合，表示在当前阶段之前已被聚合并移除的维度。$ A \cap (S_{\text{block}} \cup S_{\text{parallel}}) = \emptyset $。
- **$S_{\text{pending}}$**：待聚合维度集合，表示当前阶段结束时会被聚合的维度。$ P_{\text{pending}} \subseteq S_{\text{block}} $。
- **$S_{\text{expand}}$**：扩展维度集合，表示当前阶段结束时会新增的维度。$ E \cap (S_{\text{block}} \cup S_{\text{parallel}} \cup A) = \emptyset $。
- **$S_{\text{parallel}}$**：并行维度集合，表示当前阶段可以独立并行处理的维度。$ S_{\text{parallel}} = D \setminus (A \cup S_{\text{block}}) $。
- **$\text{Index}$**：当前块在并行维度上的索引，用于标识数据块的位置或任务划分。
- **$f$**：计算函数或操作逻辑，作用于当前阶段的数据块。$f$具有性质$P$。
    - **$P = \text{Aggregation}$**：表示 $f$ 是一个聚合操作（如求和、平均等）。
    - **$P = \text{Expansion}$**：表示 $f$ 是一个扩展操作（如新增维度）。
    - **$P = \text{Identity}$**：表示 $f$ 是一个恒等操作（即不改变维度）。

#### 约束条件

- 待聚合维度与扩展维度的互斥性，待聚合维度 $S_{\text{pending}}$ 与扩展维度 $S_{\text{expand}}$ 必须互斥：
$$
S_{\text{pending}} \cap S_{\text{expand}} = \emptyset.
$$

- **如果 $P = \text{Aggregation}$**：
  $$
  S_{\text{pending}} \neq \emptyset \quad \text{且} \quad S_{\text{expand}} = \emptyset.
  $$

- **如果 $P = \text{Expansion}$**：
  $$
  S_{\text{pending}} = \emptyset \quad \text{且} \quad S_{\text{expand}} \neq \emptyset.
  $$

- **如果 $P = \text{Identity}$**：
  $$
  S_{\text{pending}} = S_{\text{expand}} = \emptyset.
  $$

- 待聚合维度必须是块维度的子集：
  $$
  S_{\text{pending}} \subseteq S_{\text{block}}.
  $$

- 扩展维度与块维度必须互斥：
  $$
  S_{\text{expand}} \cap S_{\text{block}} = \emptyset.
  $$

- 并行维度 $S_{\text{parallel}}$ 必须与块维度、已聚合维度、待聚合维度互斥：
$$
S_{\text{parallel}} \cap (S_{\text{block}} \cup S_{\text{aggregated}} \cup S_{\text{pending}}) = \emptyset.
$$

- 必须是全划分
$$
    D = S_{\text{block}}  \cup  S_{\text{aggregated}} \cup S_{\text{parallel}}.
$$

- 扩展维度是一个特殊的维度，它在被使用的时候维度大小必须是1，这个维度要么本来就是1且在并行维度里，否则只能是已聚合维度中的一个。
$$
    (E \subseteq S_{\text{parallel}} \land |E| = 1) \lor (E \subseteq A).
$$


### $ Command $ 定义

$$
C = \{O_1, O_2, \dots, O_k\},
$$

**$Command$** 是多个 $Operator$ 的有序集合，其中每个 $O_i$ 是一个 $Operator$，且满足以下约束条件：

1. **维度一致性**：
   所有 $Operator$ 的 $S_{\text{block}}$、$S_{\text{aggregated}}$ 和 $S_{\text{parallel}}$ 必须一致。

2. **空约束**：
   除了最后一个 $Operator$ 以外，其它的所有$Operator$ 的 $S_{\text{pending}}$ 或 $S_{\text{expand}}$ 必须为 $\emptyset$。

#### 实现优势

- 高效性

    由于内部的 $Operator$ 数据结构完全一致，执行时可以直接对同一个数据块进行操作，减少了数据拷贝或维度变换的开销。

- 并行友好性

    $S_{\text{parallel}}$ 确定了数据并行的维度，因此在 $Command$ 的并行执行过程中无需担心维度冲突。

- 灵活性

    虽然内部的维度约束较为严格，但允许最后一个 $Operator$ 扩展或聚合维度，赋予 $Command$ 足够的灵活性来完成复杂的阶段性任务。
