import pandas as pd
from sklearn.datasets import load_diabetes

pd.options.display.max_columns = 100

data = load_diabetes()

X = pd.DataFrame(data.data, columns=data.feature_names)
y = pd.Series(data.target)

X.info()
X.describe()
# 이미 정규화 되어 있는 데이터 값 -> 선형 모델에 유리함
X.isnull().sum()

from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y, test_size=0.3, random_state=1)

from sklearn.preprocessing import PolynomialFeatures

poly = PolynomialFeatures(degree=3, include_bias=False).fit(X)
X_poly = poly.transform(X)
X_poly_train = poly.transform(X_train)
X_poly_test = poly.transform(X_test)

from sklearn.ensemble import BaggingRegressor, GradientBoostingRegressor
from sklearn.tree import DecisionTreeRegressor
from sklearn.linear_model import LinearRegression
from sklearn.metrics import mean_absolute_error

bag_model = BaggingRegressor(
    base_estimator=DecisionTreeRegressor(
        random_state=1
    ),
    n_estimators=100,
    max_samples=0.3,
    max_features=0.5,
    random_state=1,
    n_jobs=6
)

# lbag_model = BaggingRegressor(
#     base_estimator=LinearRegression(),
#     n_estimators=10,
#     max_samples=1.0,
#     max_features=1.0,
#     random_state=1,
#     n_jobs=6
# )

gb_model = GradientBoostingRegressor(
    n_estimators=10,
    learning_rate=0.1,
    subsample=0.3,
    max_depth=5,
    random_state=1,
)

bag_model.fit(X_train, y_train)
# lbag_model.fit(X_poly_train, y_train)
gb_model.fit(X_train, y_train)

bag_model.score(X_train, y_train)
# lbag_model.score(X_poly_train, y_train)
gb_model.score(X_train, y_train)

bag_model.score(X_test, y_test)
gb_model.score(X_test, y_test)

mean_absolute_error(y, bag_model.predict(X))
# mean_absolute_error(y, lbag_model.predict(X_poly))
mean_absolute_error(y, gb_model.predict(X))
