# -*- coding: utf-8 -*-

import pandas as pd
from sklearn.datasets import load_breast_cancer

data = load_breast_cancer()

X = pd.DataFrame(data.data, columns=data.feature_names)
y = pd.Series(data.target)

pd.options.display.max_columns=100
X.head()

from sklearn.model_selection import train_test_split
X_train,X_test,y_train,y_test=train_test_split(X,y,
                                               test_size=0.3,
                                               stratify=y,
                                               random_state=1)


# 앙상블 기반의 클래스를 로딩
from sklearn.ensemble import BaggingClassifier

# 앙상블을 구성하는 각 모델의 클래스를 로딩
# - 배깅의 경우 앙상블을 구성하는 머신러닝 모델은
#   1개를 사용함
# - 다만 각각의 모델이 학습하는 데이터는 
#   무작위 추출 방법(부트스트래핑)으로 처리하여
#   다수개의 모델이 서로다른 관점의 학습을 진행할 수 있도록 처리
from sklearn.tree import DecisionTreeClassifier

base_estimator = DecisionTreeClassifier(random_state=1)

model = BaggingClassifier(base_estimator=base_estimator,
                          n_estimators=10,
                          max_samples=0.5,
                          max_features=0.3,
                          random_state=1,
                          n_jobs=-1)

model.fit(X_train, y_train)

# 평가
score = model.score(X_train, y_train)
print(f'Score (train) : {score} ')

score = model.score(X_test, y_test)
print(f'Score (test) : {score} ')
