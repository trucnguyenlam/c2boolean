funcPrefixChange = {}
funcPrefixChange['__VERIFIER_atomic'] = '__CS_atomic'

changeID = {}

changeID['pthread_t'] = '__cs_t'  
changeID['pthread_create'] = '__cs_create'  
changeID['pthread_exit'] = '__cs_exit'  
changeID['pthread_self'] = '__cs_self'
changeID['pthread_join'] = '__cs_join'

changeID['PTHREAD_MUTEX_INITIALIZER'] = '__cs_MUTEX_INITIALIZER'
changeID['pthread_mutex_t'] = '__cs_mutex_t'
changeID['pthread_mutex_init'] = '__cs_mutex_init'
changeID['pthread_mutex_destroy'] = '__cs_mutex_destroy'
changeID['pthread_mutex_lock'] = '__cs_mutex_lock'
changeID['pthread_mutex_unlock'] = '__cs_mutex_unlock'

changeID['PTHREAD_COND_INITIALIZER'] = '__cs_COND_INITIALIZER'
changeID['pthread_cond_t'] = '__cs_cond_t'
changeID['pthread_condattr_t'] = '__cs_condattr_t'
changeID['pthread_cond_init'] = '__cs_cond_init'
changeID['pthread_cond_wait'] = '__cs_cond_wait'
changeID['pthread_cond_signal'] = '__cs_cond_signal'

changeID['malloc'] = '__cs_safe_malloc'   # or change to __cs_unsafe_malloc if you want to consider when malloc()s fails!



