4,7之前，用的多线程去做任务 (100%);
4,8之后，采用半同步半异步线程池来隔离添加任务线程和任务worker线程，因为后面会扩充一些业务逻辑 (100%);
4,9之后，使用ioc和基于动态织入的轻量级aop，使用AOP还有一种情况是为了提高程序的可维护性，将程序的非核心逻辑都“横切”出来，将非核心逻辑和核心逻辑分离，比如日志，认证等 (100%);

