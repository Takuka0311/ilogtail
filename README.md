# iLogtail社区 通义灵码助力开源大赛 比赛说明

## 赛事说明

### 背景

在数字化转型加速的时代，开源软件已成为技术创新的重要基石，支撑着全球数以百万计的应用和服务。维护和优化这些项目，确保其质量和可持续性成为前所未有的挑战。

iLogtail 作为一款监控和日志采集的开源软件，自发布以来得到了广大开发者的支持和贡献，如今已经发展到了2.0版本。在版本迭代和功能拓展的过程中，我们发现 iLogtail 针对特定格式（例如nginx、json、分隔符）的日志解析性能尚未达到理想状态。考虑到这些特定格式在日志和数据处理中的广泛应用，优化它们的解析和处理能力将极大地提升 iLogtail 的整体效率。

### 目标

通过使用通义灵码，发现优化点并提升以下几种日志格式的解析性能：

1. JSON日志
2. Nginx日志格式
3. 多行日志

### 参赛对象

1. 有编程基础的初学者（计算机系在校学生）
2. 有一定应用开发实践经验（初级开发工作者）
3. 有专业技术基础，有企业应用开发经验（企业开发者）

### 产出要求

* 优化方案设计：分析当前日志解析过程中存在的性能瓶颈，提出并详述具体的优化方案，包括使用新的解析库、代码优化和算法改进等。
* 代码实现和优化：根据设计方案，修改 iLogtail 的代码实现日志解析性能的提升。
* 撰写性能分析报告：使用公开的验证数据集进行测试，记录优化前后的处理时间和解析成功率。
* 技术文档：撰写和更新技术文档，记录优化过程、关键技术点及最终效果，以便社区其他开发者参考和学习。
* 提交 Pull Request：将优化后的代码和文档以 Pull Request 的方式提交到 iLogtail 开源项目中，进行社区贡献。

### 能力要求

* 扎实的编程基础：熟练使用 C++ 语言，进行高效的代码开发和优化。
* 性能分析与优化经验：具备分析和优化代码性能的实战经验，能够精准识别并解决性能瓶颈。
* 文档编写能力：能够清晰、详细地记录优化方案、过程和效果，同时撰写高质量的技术文档。
* 开源项目协作能力：具备丰富的开源项目协作经验，熟悉代码审查、提交 Pull Request 和处理社区反馈。

## 参与方式

### 比赛规则

本代码库包含一个验证用的日志数据集，包含这三类数据。参赛者需要针对这三种日志解析场景进行优化，以减少数据集的总处理时间并提高解析成功率。

此外，我们还准备了一个未公开的数据集，优化后的 iLogtail 处理该数据集的总时间和成功率将作为评定参赛者最终成绩的标准。

数据集位于。。。。

为了减少各位开发者由于开发设备性能的不同造成的影响，我们配置了自动的 Github Actions 用于运行测试。当您提交代码到 Github 时，会自动触发 Actions 并运行测试。可以在 Github 的 Actions 界面看到测试数据集运行的时间。

### 代码的拉取、测试、提交说明

#### 拉取

本代码库基于[`alibaba/ilogtail`](https://github.com/alibaba/ilogtail)库fork而来，并针对赛事进行了一些修改。

参赛者可以基于[`alibaba/ilogtail`](https://github.com/alibaba/ilogtail)库fork一个自己的仓库`<user_name>/ilogtail`，拉取到本地，然后手动将该代码库为一个新的远程仓库添加到现有的 fork 中。

```shell
# 拉取fork的代码
git clone https://github.com/<user_name>/ilogtail.git
cd ilogtail
git fetch origin
# 添加远程仓库
git remote add contest https://github.com/iLogtail/ilogtail-lingma-contest.git
git fetch contest
```

#### 开发

从`contest/main`分支创建一个本地的开发分支

```shell
git checkout -b <my-feature-branch> contest/main
```

进行开发并提交

```shell
# 编辑您的代码...

# 逐步添加更改
git add .

# 提交更改
git commit -m "你的提交信息"
```

#### 提交与创建pr

推送本地的开发分支到您fork的代码库（注意这里就是origin，不是contest）

```shell
git push origin <my-feature-branch>
```

然后提交 PR：

* 打开浏览器，访问 fork 仓库 `<user_name>/ilogtail`
* 找到`<my-feature-branch>`分支，点击`Compare & pull request`，并选择 `iLogtail/ilogtail-lingma-contest` 仓库的 main 分支作为目标。
* 填写 PR 的标题和描述，详细描述您的更改内容。
* 点击 `Create pull request` 完成 PR 的创建。

请注意，代码是往 `iLogtail/ilogtail-lingma-contest` 仓库提交，而不是向 `alibaba/ilogtail` 提交。

#### 代码的合入

获胜的参赛者的pr不会直接合入 `iLogtail/ilogtail-lingma-contest` 的主分支，需要基于 `alibaba/ilogtail` 的 main 分支创建新分支，将获胜代码复制到该分支，并向 `alibaba/ilogtail` 仓库的 main 分支创建 PR，完成最终的 review 与合入。
