# originaly code written by: Jannik Reichert
# can be executed on commandline with: R - f filename --no-save

library(ggplot2) # For graph plotting

# Set working directory
setwd("./")

# Read data from csv files
o <- read.table("length_all.csv",
                header = FALSE,
                sep = ";",
                dec = "."
                )
p <- read.table("length_ausschnitt.csv",
                header = FALSE,
                sep = ";",
                dec = "."
)

# Plot data
ggplot(o, aes(x = V1)) + geom_histogram() + scale_y_log10() + labs(x = "IP-Datagramm-Größe in Byte", y = "Dekadisch-logarithmische Häufigkeit")
ggsave("length_all.pdf")
ggplot(p, aes(x = V1)) + geom_histogram() + scale_y_log10() + labs(x = "IP-Datagramm-Größe in Byte", y = "Dekadisch-logarithmische Häufigkeit")
ggsave("length_ausschnitt.pdf")
