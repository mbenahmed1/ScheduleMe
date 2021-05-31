import matplotlib.pyplot as plt
import matplotlib as mpl
import seaborn as sns
import pandas as pd

#sns.set()

#sns.set_style("darkgrid")
sns.set_theme(style="whitegrid")
sns.set_palette("pastel")
# jede instanzgröße und zeit ein balken


# j30 = pd.read_csv("phase_1_j30.csv")
# j60 = pd.read_csv("phase_1_j60.csv")
# j120 = pd.read_csv("phase_1_j120.csv")

j30 = pd.read_csv("phase_1_wnbh_j30.csv")
j60 = pd.read_csv("phase_1_wnbh_j60.csv")
j120 = pd.read_csv("phase_1_wnbh_j120.csv")


# j30["makespan"] = j30["opt"] / j30["makespan"]
# j60["makespan"] = j60["opt"] / j60["makespan"]
# j120["makespan"] = j120["opt"] / j120["makespan"]

j30["makespan"] = (j30["makespan"] / j30["opt"] - 1) * 100
j60["makespan"] = (j60["makespan"] / j60["opt"] - 1) * 100
j120["makespan"] = (j120["makespan"] / j120["opt"] - 1) * 100

# j30 = j30.loc[j30["nbh"]=="api"]
# j60 = j60.loc[j60["nbh"]=="api"]
# j120 = j120.loc[j120["nbh"]=="api"]

j30_avgseed = j30.groupby(["instance_nr", "time", "nbh"])["makespan"].mean().reset_index("instance_nr").reset_index("time").reset_index("nbh")
j60_avgseed = j60.groupby(["instance_nr", "time", "nbh"])["makespan"].mean().reset_index("instance_nr").reset_index("time").reset_index("nbh")
j120_avgseed = j120.groupby(["instance_nr", "time", "nbh"])["makespan"].mean().reset_index("instance_nr").reset_index("time").reset_index("nbh")


j30_time = j30_avgseed.groupby(["time", "nbh"]).mean().reset_index("time").reset_index("nbh")
j30_time["instance"] = "j30"

j60_time = j60_avgseed.groupby(["time", "nbh"]).mean().reset_index("time").reset_index("nbh")
j60_time["instance"] = "j60"

j120_time = j120_avgseed.groupby(["time", "nbh"]).mean().reset_index("time").reset_index("nbh")
j120_time["instance"] = "j120"

# j30_time = j30_avgseed.groupby("time").mean().reset_index("time")
# j60_time = j60_avgseed.groupby("time").mean().reset_index("time")
# j120_time = j120_avgseed.groupby("time").mean().reset_index("time")

# j30_time["instance"] = "j30"
# j60_time["instance"] = "j60"
# j120_time["instance"] = "j120"

# j_all = pd.concat([j30_avgseed, j60_avgseed, j120_avgseed])

j_time = pd.concat([j30_time, j60_time, j120_time])

print(j_time.head(10))

#sns.barplot(x=j30_avgseed.instance_nr, y=j30_avgseed.makespan, hue=j30_avgseed.time)

fig, axes = plt.subplots(1, 2, figsize=(18, 10))

api = j_time.loc[j_time["nbh"]=="api"]
swap = j_time.loc[j_time["nbh"]=="swap"]




sns.barplot(ax=axes[0], x=api.instance,  y = api.makespan, hue=api.time)
sns.barplot(ax=axes[1], x=swap.instance,  y = swap.makespan, hue=swap.time)

axes[0].set(ylim=(0.0, 20), title="API", xlabel="Instanz", ylabel="Abweichung zur Optimalen Lösung [%]")
axes[1].set(ylim=(0.0, 20), title="SWAP", xlabel="Instanz", ylabel="Abweichung zur Optimalen Lösung [%]")

# sea = sns.FacetGrid(j_time, col = "nbh", hue="time")
  
# sea.map(sns.barplot, "instance", "makespan",
#         order = ["j30", "j60", "j120"])

plt.tight_layout()
plt.show()

