"""
앙상블 (Ensemble)
    - 다수개의 모델을 결합하여 하나의 예측을 할 수 있는 결합 모델
    - 다수결의 원칙이 적용 (분류)
        - 일반화 성능(테스트 데이터에 대한 성능)을 극대화하기 위해서 생성된 모델이다.
    - 평균 원칙이 적용(회귀)


앙상블은 가장 좋은 평가 성적을 반환하지 않음
    - 앙상블은 하나의 모델 객체를 사용하지 않고
      다수개의 모델결과를 사용하여 다수결/평균을 취하므로
      앙상블을 구성하는 많은 모델에서 가장 좋은 성적의
      모델보다는 항상 평가가 떨어질 수 있음!
    - 일반화 성능을 극대화하는 모델임
    (테스트 데이터에 대한 성능)

앙상블을 구현하는 방법
    1. 취합모델(aggregation model)
        - 앙상블 구성하고 있는 각각의 모델이 **독립적**으로 동작
        - 각각의 모델이 독립적으로 학습하고 예측한 결과를 반환하여
          최종적으로 취합된 결과를 다수결 / 평균으로 예측함
        - Voting(다수결 방식)
        - Bagging
        - RandomForest
        - 병렬처리를 통해 학습과 예측의 속도를 매우 빠르게 올릴 수 있다.

    2. boosting
        - 취합이랑 반대
        - 앙상블을 구성하는 각각의 모델이 선형으로 결합되어
          점진적으로 학습의 성능을 향상시켜 나가는 방법
        - 부스팅의 첫번째 모델이 예측 결과 *
        - AdaBoosting, GradientBoosting(성능은 좋으나 속도가 느려서 사용되지 않음)
        - **"XGBoost"**(GradientBoosting를 병렬처리를 붙임!)
        - **"LightGBM"**(현업에서 제일 자주 쓰는 방식)
        - Boosting model 은 강한 제약 조건을 설정하여 점진적으로
          성능이 향상될 수 있도록 제어한다.
        - 모델이 독립적이지 않아 병렬처리가 불가능해서 학습시간이 매우 오래걸림

    시험 문제!
    - 10개의 모델의 취합 모델
        - 각 모델은 **과적합**시키는게 오히려 좋다!
        - 어차피 평균 낼 것이기 때문!
    - Boosting은 제약조건을 심하게 걸어서 학습시킨다.
"""

# base code
import pandas as pd
from sklearn.datasets import load_breast_cancer

data = load_breast_cancer()

X = pd.DataFrame(data.data, columns=data.feature_names)
y = pd.Series(data.target)

from sklearn.model_selection import train_test_split

X_train, X_test, y_train, y_test = train_test_split(X, y,
                                                    test_size=0.3,
                                                    stratify=y,
                                                    random_state=1)

# ensemble 기반의 클래스를 로딩
# - 부스팅 : 내부의 모델들이 선형으로 결합되어 순차적으로 학습/예측을 수행하는 모델
#               - 약한 학습을 여러 번 시키는 방법
# - AdaBoosting :       데이터의 관점으로 성능을 향상시켜 나가는 방법,
#                       내가 앞서본 데이터 중 틀린 데이터에 가중치를 높여 성능을 향상시키는 방법
# - GradientBoosting :  오차의 관점에서 성능을 향상시켜 나가는 방법
#                       틀린 정도(잔여오차)에 따라서 가중치가 달라짐
#                       (Ada boost에 비해서 상대적으로 학습할 수 있음)
from sklearn.ensemble import AdaBoostClassifier

# 앙상블을 구성하는 각 모델의 클레스를 로딩
from sklearn.linear_model import LogisticRegression

# Alpha = 1/C, C는 높아질 수록 제약이 낮아짐
base_estimator = LogisticRegression(C=0.00001, random_state=1, n_jobs=-1)

model = AdaBoostClassifier(
    base_estimator=base_estimator,
    n_estimators=50,
    learning_rate=1.5,
    random_state=1
)
# random_state는 항상 일정한 수를 넣어줘야지 성능 분석하기가 쉬움!

model.fit(X_train, y_train)

score = model.score(X_train, y_train)

t_score = model.score(X_test, y_test)

pred = model.predict(X_test[:1])

pred = model.estimators_[0].predict(X_test[:1])
