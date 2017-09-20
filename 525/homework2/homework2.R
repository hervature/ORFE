set.seed(525)

data <- read.csv("MLB2008.csv")

data.train <- data[1:154,]
data.test <- data[155:nrow(data),]

# Question 1a
library('rpart')
library('rpart.plot')

## Everything to the right of POS
x_names <- names(data)[6:134]
formula <- paste("SALARY ~ ", paste(x_names, collapse= "+"))

## No prune
data.rpart.model <- rpart(formula, data.train)
dev.new()
prp(data.rpart.model)

## MSE no prune
data.rpart.train.predict <- predict(data.rpart.model, data.train)
mse <- norm(data.rpart.train.predict - data.train$SALARY, type="2")
sprintf("MSE for trained data: %0.0f", mse)

data.rpart.test.predict <- predict(data.rpart.model, data.test)
mse <- norm(data.rpart.test.predict - data.test$SALARY, type="2")
sprintf("MSE for test data: %0.0f", mse)

## Prune parameter of 0.1
data.rpart.prune.model <- prune(data.rpart.model, cp=0.1)
dev.new()
prp(data.rpart.prune.model)

## MSE no prune
data.rpart.prune.train.predict <- predict(data.rpart.prune.model, data.train)
mse <- norm(data.rpart.prune.train.predict - data.train$SALARY, type="2")
sprintf("MSE for trained data (pruned): %0.0f", mse)

data.rpart.prune.test.predict <- predict(data.rpart.prune.model, data.test)
mse <- norm(data.rpart.prune.test.predict - data.test$SALARY, type="2")
sprintf("MSE for test data (pruned): %0.0f", mse)

## Prune parameter of 0.3
data.rpart.prune.model <- prune(data.rpart.model, cp=0.3)
dev.new()
prp(data.rpart.prune.model)

## MSE no prune
data.rpart.prune.train.predict <- predict(data.rpart.prune.model, data.train)
mse <- norm(data.rpart.prune.train.predict - data.train$SALARY, type="2")
sprintf("MSE for trained data (pruned): %0.0f", mse)

data.rpart.prune.test.predict <- predict(data.rpart.prune.model, data.test)
mse <- norm(data.rpart.prune.test.predict - data.test$SALARY, type="2")
sprintf("MSE for test data (pruned): %0.0f", mse)

# Question 1b
library('randomForest')

random_forest.mse.train <- data.frame(B=integer(), mse=numeric()) 
random_forest.mse.test <- data.frame(B=integer(), mse=numeric()) 

for(B in seq(10, 200, by=5)) {
    data.random_forest.model <- randomForest(as.formula(formula), data=data.train, ntree=B)
    df <- data.frame(B, mse=norm(predict(data.random_forest.model, data.train) - data.train$SALARY, type="2"))
    random_forest.mse.train <- rbind(random_forest.mse.train, df)
    df <- data.frame(B, mse=norm(predict(data.random_forest.model, data.test) - data.test$SALARY, type="2"))
    random_forest.mse.test <- rbind(random_forest.mse.test, df)
}

dev.new()
plot(random_forest.mse.train,type="l",col="red", ylim=range(c(random_forest.mse.train["mse"],random_forest.mse.test["mse"])))
par(new = TRUE)
plot(random_forest.mse.test,type="l",col="blue", ylim=range(c(random_forest.mse.train["mse"],random_forest.mse.test["mse"])))

# Question 1c

## Min MSE on test data is random forest with B=45 (predict on full data set)
data.random_forest.model <- randomForest(as.formula(formula), data=data.train, ntree=45)
data.random_forest.full <- predict(data.random_forest.model, data)

## Most undervalued
data.random_forest.undervalued <- (data.random_forest.full - data$SALARY)
undervalued <- which.max(data.random_forest.undervalued)
undervalued.player <- data[c("PLAYER","SALARY")][undervalued,]
sprintf("Most undervalued player: %s, Actual: $%0.0f, Predicted: $%0.0f", undervalued.player$PLAYER, undervalued.player$SALARY, data.random_forest.undervalued[undervalued])