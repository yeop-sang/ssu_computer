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
# - 부스팅 : 내부의 모델들이 선형으로 결합되어
#           순차적으로 학습/예측을 수행하는 모델

# - AdaBoosting : 데이터의 관점으로 성능을 향상시켜 나가는 방법
# - GradientBoosting : 오차의 관점에서 성능을 향상시켜 나가는 방법

from sklearn.ensemble import AdaBoostClassifier

from sklearn.linear_model import LogisticRegression
base_estimator = LogisticRegression(C=0.0001,
                                    n_jobs=-1,
                                    random_state=1)

model = AdaBoostClassifier(base_estimator=base_estimator,
                           n_estimators=150,
                           learning_rate=1.9,
                           random_state=1)

model.fit(X_train, y_train)

# 평가
score = model.score(X_train, y_train)
print(f'Score (train) : {score} ')

score = model.score(X_test, y_test)
print(f'Score (test) : {score} ')










